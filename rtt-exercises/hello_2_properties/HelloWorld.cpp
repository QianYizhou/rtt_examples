
/**
 * @file HelloWorld.cpp
 * This file demonstrates the Orocos 'Property' and 'Attribute' primitives with
 * a 'hello world' example.
 */

#include <rtt/os/main.h>

#include <rtt/Logger.hpp>
#include <rtt/TaskContext.hpp>
#include <rtt/Activity.hpp>

/**
 * Include this header in order to use properties or attributes.
 */
#include <rtt/Property.hpp>
#include <rtt/Attribute.hpp>
#include <rtt/Component.hpp>
#include <rtt/marsh/Marshalling.hpp>

using namespace std;
using namespace RTT;

/**
 * Exercise 2: Read Orocos Component Builder's Manual, Chap 2 sect. 3.6
 *
 * First, compile and run this application and use 'property' and 'attribute':
 * Change and print their values in the TaskBrowser.
 *
 * Next save the properties of this component to a hello.xml file:
 * You will need to install the 'marshalling' service using the TaskBrowser
 * at runtime:
 * ** TaskBrowser: type from the Deployer: loadService("hello", "marshalling")
 *
 * To make this permanent for your component, add this statement in the start.ops file.
 *
 * Optional: use loadService in C++:
 *    In C++ you need to #include <rtt/marsh/Marshalling.hpp> 
 *    and add to the constructor: this->getProvider<Marshalling>("marshalling");
 *
 *    In the CMakeLists.txt: add rtt-marshalling to the list of components to look for in
 *    find_package(OROCOS-RTT REQUIRED) macro.
 *    See : http://www.orocos.org/wiki/orocos/toolchain/getting-started/cmake-and-building
 *    and use that syntax in the CMakeLists.txt file to link 'HelloWorld' with marshalling.
 *
 *
 * When marshalling is loaded:
 * In order to find out how to write the property to a file using 'marshalling',
 * type 'marshalling' (without quotes) to see the interface of the marshalling
 * task object.
 *
 * Next Open and modify the XML file and read it back in using the marshalling object.
 *
 * For the optional exercises, reead Chap 2, sect 6.1
 * Optional : read the property file from configureHook() and log it's value. You need
 * to make the modifications detailed above in the note.
 * Optional : write the property file in cleanupHook().
 *
 * For ROS users: load the rtt_rosparam service as well and send the properties to the
 * ROS master server instead of to the XML file. 
 * At runtime:
 * ** TaskBrowser: type 'import("rtt_rosnode")' and '.provide rosparam' in "hello"
 *
 * Open question: Would you prefer to hard-code this property reading/writing or would
 * you prefer to script it ?
 */
namespace Example
{

    /**
     * Every component inherits from the 'TaskContext' class.  This base
     * class allow a user to add a primitive to the interface and contain
     * an ExecutionEngine which executes application code.
     */
    class Hello
        : public RTT::TaskContext
    {
    protected:
        /**
         * @name Name-Value parameters
         * @{
         */
        /**
         * Properties take a name, a value and a description
         * and are suitable for XML.
         */
        std::string property;
        /**
         * Attributes are C++ variables exported to the interface.
         */
        double attribute;
        /**
         * Constants are not changeable by an outsider, only by us.
         */
        std::string constant;
        /** @} */

    public:
        /**
         * This example sets the interface up in the Constructor
         * of the component.
         */
        Hello(std::string name)
            : TaskContext(name),
              // Name, description, value
              property("Hello World")
        {
            // Now add it to the interface:
            this->addProperty("property", property).doc("This property can contain any friendly string.");

            this->addAttribute("attribute", attribute);
            this->addConstant("constant", constant);
        }

        void updateHook() override {
            static std::string last_property = property;
            static double last_attribute = attribute;
            static std::string last_constant = constant;

            if (last_property != property) {
                printf("Property changed from %s to %s\n", last_property.c_str(), property.c_str());
                last_property = property;
            }
            if (std::fabs(last_attribute - attribute) > 1e-6) {
                printf("Attribute changed from %f to %f\n", last_attribute, attribute);
                last_attribute = attribute;
            }
            if (last_constant != constant) {
                printf("Constant changed from %s to %s\n", last_constant.c_str(), constant.c_str());
                last_constant = constant;
            }
            // printf("updateHook() called ! property: %s, attribute: %f, constant: %s\n", property.c_str(), attribute, constant.c_str());

            static int index = 0;

            printf("updateHook called at index: %d\n", ++index);
            log(Info) << "Update !" << index <<endlog();

            if (index == 3) {
                sleep(5);
            }
        }
    };

    class MyTask : public RTT::TaskContext
    {
        int i_param = 5;
        double d_param = -3.0;
        RTT::PropertyBag sub_bag;
        std::string s_param = "The String";
        bool b_param = false;
    public:
        MyTask(std::string name)
            : TaskContext(name)
        {
            this->addProperty("i_param", i_param).doc("An integer parameter");
            this->addProperty("d_param", d_param).doc("A double parameter");
            this->addProperty("SubBag", sub_bag).doc("A sub-bag");
            sub_bag.addProperty("s_param", s_param).doc("A string parameter");
            sub_bag.addProperty("b_param", b_param).doc("A boolean parameter");

            this->getProvider<Marshalling>("marshalling")->writeProperties("hello2.xml");
        }
    };
}

ORO_CREATE_COMPONENT_LIBRARY()
ORO_LIST_COMPONENT_TYPE( Example::Hello )
ORO_LIST_COMPONENT_TYPE( Example::MyTask )
