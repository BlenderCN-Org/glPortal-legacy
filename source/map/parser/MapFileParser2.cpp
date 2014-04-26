#include "MapFileParser2.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "../../GameMap.hpp"
#include "../../engine/Resources.hpp"
#include "../../engine/StringConverter.hpp"
#include "../../engine/Environment.hpp"
#include "../../Exception.hpp"
#include "../../util/Vector3f.hpp"
#include "EventType.hpp"
#include "ParserState.hpp"
#include "SyntaxConstraintFactory.hpp"

namespace glPortal {
  namespace map{
    namespace parser{
      /**
       * Get a map from a map file.
       *
       * @param filename Path to the map file
       */
      GameMap MapFileParser2::getMapFromFile(std::string filename) {
        initializeSyntax();
        this->fileName = filename;
        std::ifstream fileStream(filename, std::ifstream::in);

        this->gameMap.flush();

        if(!fileStream){
          cout << NO_FILE_MESSAGE;
          this->gameMap.setIsLastScreen();
          return this->gameMap;
        }
      
        while(fileStream.good()) {
          state = ParserState::READING_COMMAND;
          this->parse(fileStream);
          fileStream.close();
          return this->gameMap;
        }
      }
      
      void MapFileParser2::initializeSyntax(){
        characterConstraints = SyntaxConstraintFactory::getCLikeCharacterMap();
      }

      void MapFileParser2::parse(std::ifstream &fileStream){
        std::string line, string;
        while(std::getline(fileStream, line)){
          lineNumber++;
          currentPositionMessage = "On line " + std::to_string(lineNumber) + " in file " + this->fileName + ".";
          std::istringstream iss(line);

          if(line.length() > 0) {
            for(int i = 0; i <= line.length(); i++){
              stringstream conversionStream;
              conversionStream << line[i];
              if(line[i] == ' '){
                currentCharacter = std::string(" ");
              } else {
                conversionStream >> currentCharacter;
              }
              this->tokenize();
            }
          }
        }        
      }

      void MapFileParser2::tokenize(){
        std::string message;
        ParserState newState;
        if((characterConstraints.find(currentCharacter) != characterConstraints.end())){
          cout << "\n";
          std::vector<SyntaxConstraint> constraintVector = characterConstraints.at(currentCharacter);
          bool hasValidState = false;
          if(Environment::DEBUG){
            cout << "char found in map." << "\n";
            cout << "char: " << currentCharacter << "\n";
            cout << "before_state: " << parserStateStrings[(int)state] << "\n";
          }
          for (int i=0; i<constraintVector.size(); i++) {
            SyntaxConstraint constraint = constraintVector[i];
            if(Environment::DEBUG){
              cout << "state: " << parserStateStrings[(int)state] << "\n";
            }
            if(constraint.getIsValidPrerequisiteState(state)){
              hasValidState = true;
              if(Environment::DEBUG){
                cout << "-> Detected valid state. \n";
              }
              std::vector<EventType> events = constraint.getEvents();

              for(std::vector<EventType>::iterator event = events.begin(); event != events.end(); ++event) {
                executeEvent(*event);
                if(Environment::DEBUG){
                  cout << "switching: " << parserStateStrings[(int)state] << " to ";
                  cout << parserStateStrings[(int)constraint.getResultState()] << "\n";
                }
              }
              newState = constraint.getResultState();
            }         
          } 
          
          if(newState == ParserState::PREVIOUS_STATE){
          } else {
            state = newState;
          }

          if((!hasValidState) && (skipLine == false)){
            throwException();
          }
        } else if((currentCharacter != " ") && (skipLine == false)){
          stringStack += currentCharacter; 
        }
      }

      void MapFileParser2::executeEvent(EventType event){
        switch(event){
        case EventType::COPY_BUFFER_TO_COMMAND:
          //                                cout << "copy";
          command = stringStack;
          break;
        case EventType::CLEAR_BUFFER:
          //                                cout << "clear";
          clearStringStack();
                  break;
        case EventType::EXECUTE:
          if(Environment::DEBUG){
            cout << "command found: " << command << "\n";
          }
          break;
        default:
          //                cout << "default\n";
          //                cout << (int)events[i] << "xxx";
          //                cout << (int)*event;
          break;
        }
      }

      void MapFileParser2::executeCurrentCommand(){
        if(command == "setSpawn"){        
          cout << "par" << "\n";
          cout << command;
          cout << parameters.at(0);
          cout << parameters.at(1);
          cout << "\n";
        } else {
          cout << "par" << "\n";
          cout << command;
          cout << parameters.at(0);
          cout << parameters.at(1);
          cout << "\n";
        }
        cout << "clearing" << "\n";
        parameters.clear();
      }

      void MapFileParser2::clearStringStack(){
        stringStack = "";      
      }

      void MapFileParser2::throwException(){
        std::string message = "Got \"" + currentCharacter + "\" while state was " + parserStateStrings[(int)state] + ". "  + currentPositionMessage;
        throw Exception(message);
      }
    }
  }
}