/*!
 * Copyright (C) tkornuta, IBM Corporation 2015-2019
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*!
 * \file KeyHandlerRegistry.cpp
 * \brief Definition of methods of abstract KeyHandlerRegistry class responsible for handling the keypressed callbacks.
 * \author tkornuta
 * \date Nov 20, 2015
 */

#include <application/KeyHandlerRegistry.hpp>

namespace mic {
  namespace application {

    KeyHandlerRegistry::KeyHandlerRegistry() {
      //key_map.insert(std::make_pair((char)27, std::make_pair( "ESC - exit the program", boost::bind(&KeyHandler::functionQuit, this ) ) ) );
      extendedInputMode = false;

      // Register default key handlers.
      registerKeyhandler(27, "ESC - exits the program", &KeyHandlerRegistry::keyhandlerQuit, this);
      registerKeyhandler('h', "h - displays this list of registered key handlers", &KeyHandlerRegistry::keyhandlerDisplayOptions, this);
      registerKeyhandler('s', "s - display application status", &KeyHandlerRegistry::keyhandlerDisplayAppState, this);

      // Logger.
      registerKeyhandler(';', "; - increments the logger severity level", &KeyHandlerRegistry::keyhandlerIncrementLoggerLevel, this);
      registerKeyhandler('\'', "\' - decrements the logger severity level", &KeyHandlerRegistry::keyhandlerDecrementLoggerLevel, this);

      // Time keyhandlers.
      registerKeyhandler('-', "- - slows down the processing (multiplies the sleep interval by 1.5)", &KeyHandlerRegistry::keyhandlerSlowDown, this);
      registerKeyhandler('+', "+ - speeds up the processing (divides the sleep interval by 1.5)", &KeyHandlerRegistry::keyhandlerFastenUp, this);
      registerKeyhandler('=', "= - resets the processing time (sets the sleep interval to 1s)", &KeyHandlerRegistry::keyhandlerResetProcessingTime, this);

      // Application modes.
      registerKeyhandler('l', "l - toggles learning mode on/off", &KeyHandlerRegistry::keyhandlerToggleLearning, this);
      registerKeyhandler(' ', "PAUSE - stops/starts the continuous execution of the program", &KeyHandlerRegistry::keyhandlerPause, this);
      registerKeyhandler('\\', "\\ - performs a single step", &KeyHandlerRegistry::keyhandlerSingleStep, this);

      LOG(LSTATUS) << "keyHandlerRegistry established ok...";
    }

    KeyHandlerRegistry::~KeyHandlerRegistry() {
      // TODO Auto-generated destructor stub
    }

    void KeyHandlerRegistry::keyboardHandler(unsigned char key_) {
      LOG(LTRACE) << "KeyHandlerRegistry::keyboardHandler";

      bool processKeystroke = true;
      if (extendedInputMode) {
        if (key_ == 13) {
          key_ = finishKey;
        }
        else if (key_ == 27) {
          payload = "-1"; // must be interpreted as an abort entry
          key_ = finishKey;
        }
        else {
          payload.append((const char *) &key_, 1);
          LOG(LSTATUS) << payload ;
          key_ = 0;
          processKeystroke = false;
        }
      }
      if (processKeystroke) {
        // Try to find the pressed key.
        KeyHandlerMap::const_iterator iter = key_handler_map.find(key_);
        if (iter != key_handler_map.end())
          // If found - execute the associated function.
          ((iter->second).second)();
        else
          LOG(LWARNING) << "Handler for given key not found. Please press h for help.";
      }
    }

    void KeyHandlerRegistry::keyhandlerDisplayOptions(void) {
      LOG(LTRACE) << "KeyHandlerRegistry::keyhandlerDisplayOptions";

      LOG(LSTATUS) << "----------------------------------------------------------------";
      LOG(LSTATUS) << "List of registered key handlers:";
      LOG(LSTATUS) << "----------------------------------------------------------------";
      // Iterate through handlers and display their descriptions.
      for (KeyHandlerMap::const_iterator iter = key_handler_map.begin(); iter != key_handler_map.end(); iter++) {
        LOG(LSTATUS) << (iter->second).first;
      }//: each option
      LOG(LSTATUS) << "----------------------------------------------------------------";
    }

    void KeyHandlerRegistry::keyhandlerQuit(void) {
      LOG(LTRACE) << "KeyHandlerRegistry::keyhandlerQuit";
      // Set  quit flag to true.
      APP_STATE->setQuit();

    }

    void keyhandlerDisplayAppState(void);

    void KeyHandlerRegistry::keyhandlerIncrementLoggerLevel(void) {
      LOG(LTRACE) << "KeyHandlerRegistry::keyhandlerIncrementLoggerLevel";
      LOGGER->incrementSeverityLevel();
    }

    void KeyHandlerRegistry::keyhandlerDecrementLoggerLevel(void) {
      LOG(LTRACE) << "KeyHandlerRegistry::keyhandlerDecrementLoggerLevel";
      LOGGER->decrementSeverityLevel();
    }

    void KeyHandlerRegistry::keyhandlerToggleLearning(void) {
      LOG(LTRACE) << "KeyHandlerRegistry::keyhandlerToggleLearning";
      // Switch state of the learning mode.
      APP_STATE->pressLearning();
    }

    void KeyHandlerRegistry::keyhandlerPause(void) {
      LOG(LTRACE) << "KeyHandlerRegistry::keyhandlerPause";
      // Switch state of the pause mode.
      APP_STATE->pressPause();
    }

    void KeyHandlerRegistry::keyhandlerSingleStep(void) {
      LOG(LTRACE) << "KeyHandlerRegistry::keyhandlerSingleStep";
      // Switch state of the single step mode.
      APP_STATE->pressSingleStep();
    }

    void KeyHandlerRegistry::keyhandlerSlowDown(void) {
      LOG(LTRACE) << "KeyHandlerRegistry::keyhandlerSlowDown";
      APP_STATE->multiplySleepInterval();
    }

    void KeyHandlerRegistry::keyhandlerFastenUp(void) {
      LOG(LTRACE) << "KeyHandlerRegistry::keyhandlerFastenUp";
      APP_STATE->divideSleepInterval();
    }

    void KeyHandlerRegistry::keyhandlerResetProcessingTime(void) {
      LOG(LTRACE) << "KeyHandlerRegistry::keyhandlerResetProcessingTime";
      APP_STATE->setSleepIntervalS(1);
    }

    void KeyHandlerRegistry::keyhandlerDisplayAppState(void) {
      LOG(LTRACE) << "KeyHandlerRegistry::keyhandlerDisplayAppState";
      APP_STATE->displayStatus();
    }

    std::string KeyHandlerRegistry::changeInputMode(unsigned char finishKey_) {
      if (extendedInputMode) {
        extendedInputMode = false;
      }
      else {
        extendedInputMode = true;
        finishKey = finishKey_;
        payload = "";
      }
      return (payload);
    }

  } /* namespace application */
} /* namespace mic */
