
/**
 * @file HelloWorld.cpp
 * This file demonstrates the Orocos 'Operation/OperationCaller' primitive with
 * a 'hello world' example.
 */

#include <thread>
#include <rtt/Logger.hpp>
#include <rtt/TaskContext.hpp>

/**
 * Include this header in order to call component operations.
 */
#include <rtt/OperationCaller.hpp>
#include <rtt/Component.hpp>

using namespace std;
using namespace RTT;

/**
 * Exercise 4: Read Orocos Component Builder's Manual, Chap 2 sect 3.5
 *
 * First, compile and run this application and use 'getMessage()' in the TaskBrowser.
 * Configure and start the World component ('world.start()') and see
 * how it uses getMessage(). Fix any bugs :-)
 *
 * Next, add to Hello a second method 'bool sayIt(string sentence, string& answer)'
 * which uses log(Info) to display a sentence in the thread of the Hello component.
 * When sentence is "Orocos", the answer is "Hello Friend!" and true is returned. Otherwise,
 * false is returned and answer remains untouched.
 * Add this function to the default Service of this class and document it
 * and its arguments. Create in the TaskBrowser a variable 'var string string_result'
 * and use it as the answer argument when calling sayIt("Orocos",string_result)
 *
 * Optional : Test sending and collecting arguments with the TaskBrowser. You'll
 *  have to create a 'var SendHandle sh' object in the TaskBrowser and assign to it
 *  the result of a sayIt.send("Orocos",string_result) call. Collecting the result
 *  is done using sh.collect( bool_result, string_result). Think about it why !
 *
 * Optional : Next do the same in C++. Create an OperationCaller to sayIt
 *  and call sayIt.send("Orocos", string_result) in updateHook of the World component, then
 *  collect() the string_result in the next iteration of updateHook().
 */
namespace Example
{

    /**
     * Every component inherits from the 'TaskContext' class.  This base
     * class allow a user to add a primitive to the interface and contain
     * an ExecutionEngine which executes application code.
     */
    class Hello
        : public TaskContext
    {
    protected:
        /**
         * @name Operations
         * @{
         */
        /**
         * Returns a string.
         */
        string getMessage() {
            printf("[TID: %lu] getMessage() called\n", std::hash<std::thread::id>{}(std::this_thread::get_id()));
            return "Hello World";
        }
        /** @} */

    public:
        /**
         * This example sets the interface up in the Constructor
         * of the component.
         */
        Hello(std::string name)
            : TaskContext(name)
        {
            this->addOperation("getMessage", &Hello::getMessage, this).doc("Returns a friendly word.");
            this->addOperation("getMessageOwnThread", &Hello::getMessage, this, OwnThread).doc("Returns a friendly word.");
            this->addOperation("getMessageClientThread", &Hello::getMessage, this, ClientThread).doc("Returns a friendly word.");
        }

        void updateHook() override
        {
            printf("Hello::updateHook()\n");
        }

    };

    /**
     * World is the component that shows how to call an Operation
     * of the Hello component in C++.
     */
    class World
      : public TaskContext
    {
    protected:
    	/**
    	 * This OperationCaller serves to store the
    	 * call to the Hello component's Operation.
    	 * It is best practice to have this object as
    	 * a member variable of your class.
    	 */
    	OperationCaller< string(void) > getMessage;
    	OperationCaller< string(void) > getMessageClientThread;
    	OperationCaller< string(void) > getMessageOwnThread;

    	/** @} */

    public:
    	World(std::string name)
	  : TaskContext(name, PreOperational)
    	{
            printf("World::World()\n");
    	}

    	bool configureHook() {
            log(Info) << "Configuring World component" <<endlog();
            printf("World::configureHook()\n");

            printf("Pass LINE: %d\n", __LINE__);
	    // Lookup the Hello component.
    	    TaskContext* peer = this->getPeer("hello");
    	    if ( !peer ) {
    	    	log(Error) << "Could not find Hello component!"<<endlog();
    	    	return false;
    	    }

            printf("Pass LINE: %d\n", __LINE__);

    	    // It is best practice to lookup methods of peers in
    	    // your configureHook.
    	    getMessage = peer->getOperation("getMessage");
    	    if ( !getMessage.ready() ) {
    	    	log(Error) << "Could not find Hello.getMessage Operation!"<<endlog();
    	    	return false;
    	    }
            printf("Pass LINE: %d\n", __LINE__);
    	    getMessageClientThread = peer->getOperation("getMessageClientThread");
    	    if ( !getMessageClientThread.ready() ) {
    	    	log(Error) << "Could not find Hello.getMessage Operation!"<<endlog();
    	    	return false;
    	    }
            printf("Pass LINE: %d\n", __LINE__);
    	    getMessageOwnThread = peer->getOperation("getMessageOwnThread");
    	    if ( !getMessageOwnThread.ready() ) {
    	    	log(Error) << "Could not find Hello.getMessage Operation!"<<endlog();
    	    	return false;
    	    }
            printf("Pass LINE: %d\n", __LINE__);
    	    return true;
    	}

    	void updateHook() {
            printf("[TID: %lu] World::updateHoo()\n", std::hash<std::thread::id>{}(std::this_thread::get_id()));
            // printf("World::updateHook()\n");
            // auto data = getMessage();
            // printf("World::updateHook() - data = %s\n", data.c_str());
            // printf("World::updateHook() (clientThread) - data = %s\n", getMessageClientThread().c_str());

            // This line would trigger error: possible deadlocks
            // printf("World::updateHook() (OwnThread) - data = %s\n", getMessageOwnThread().c_str());

    		log(Info) << "Receiving from 'hello': " << getMessage() <<endlog();
    		log(Info) << "Receiving from 'hello': " << getMessageClientThread() <<endlog();

            // Will send a request to the Hello component to get the message
            SendHandle<string(void)> handle = getMessageOwnThread.send();

            // Will execute in Global engine thread (another thread)
            SendHandle<string(void)> h2 = getMessageClientThread.send();

            printf("\n");
            // std::string rt_string;
            // while (handle.collectIfDone(rt_string) == RTT::SendNotReady) {
            //     sleep(1);
            // }
            // printf("World::updateHook() (OwnThread) - data = %s\n", rt_string.c_str());
    		// log(Info) << "Receiving from 'hello': " << getMessageClientThread() <<endlog();
    		// log(Info) << "Receiving from 'hello': " << getMessageOwnThread() <<endlog();
    	}
    };
}

ORO_CREATE_COMPONENT_LIBRARY()
ORO_LIST_COMPONENT_TYPE( Example::Hello )
ORO_LIST_COMPONENT_TYPE( Example::World )

