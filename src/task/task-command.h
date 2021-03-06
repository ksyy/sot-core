/*
 * Copyright 2010,
 * Florent Lamiraux
 *
 * CNRS
 *
 * This file is part of sot-core.
 * sot-core is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * sot-core is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.  You should
 * have received a copy of the GNU Lesser General Public License along
 * with sot-core.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TASK_COMMAND_H
 #define TASK_COMMAND_H

 #include <boost/assign/list_of.hpp>

 #include <dynamic-graph/command.h>
 #include <dynamic-graph/command-setter.h>
 #include <dynamic-graph/command-getter.h>

namespace dynamicgraph { namespace sot {
  namespace command {
    namespace task {
      using ::dynamicgraph::command::Command;
      using ::dynamicgraph::command::Value;
      
      // Command ListFeatures
      class ListFeatures : public Command
      {
      public:
	virtual ~ListFeatures() {}
	/// Create command and store it in Entity
	/// \param entity instance of Entity owning this command
	/// \param docstring documentation of the command
      ListFeatures(Task& entity, const std::string& docstring) :
	Command(entity, std::vector<Value::Type> (), docstring)
	  {
	  }
	virtual Value doExecute()
	{
          typedef Task::FeatureList_t FeatureList_t;
          Task& task = static_cast<Task&>(owner());
          const FeatureList_t& fl = task.getFeatureList();
          std::string result("[");
          for (FeatureList_t::const_iterator it = fl.begin ();
              it != fl.end (); it++) {
            result += "'" + (*it)->getName() + "',";
          }
          result += "]";
          return Value(result);
	}
      }; // class ListFeatures
    } // namespace task
  } // namespace command
} /* namespace sot */} /* namespace dynamicgraph */

#endif //TASK_COMMAND_H
