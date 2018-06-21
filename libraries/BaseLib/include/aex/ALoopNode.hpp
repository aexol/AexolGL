/** @file ALoopNode.hpp
 *  @brief Definition of ALoopNode.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__ALoopNode__
#define __aexolGL__ALoopNode__

#include <aex/Common.hpp>

namespace aex {

    /**
     *	@brief	ALoopNode interface.
     *  @details Interface that must be implemented by any object that will be
     *  run in main game loop.
     */
    class AEX_PUBLIC_API ALoopNode {
    public:

        /**
         *	@brief	ALoopNode constructor.
         *
         *  @details Constructs a loop node with given priority.
         *	@param 	priority 	Priority with which this node should be ran.
         */
        ALoopNode(uint32_t priority = 5000) : m_done(false), m_priority(priority) {
        }

        virtual ~ALoopNode() {
        };

    protected:
        /**
         *	@brief	Pure virtual initialization method.
         *
         *  @details This method needs to be overloaded with
         *  code specific to particualr node.
         *	@return	True if init succeeded, false otherwise.
         */

        /* Copy-Paste snippet:
         virtual bool init();
         virtual void pause();
         virtual void resume();
         virtual void update();
         virtual void finish();
         */

        virtual bool init() = 0;

        /**
         *	@brief	Pure virtual pause method.
         *  @details Needs to be oveloaded with how node behaves at pause.
         */
        virtual void pause() = 0;

        /**
         *	@brief	Pure virtual resume method.
         *  @details Needs to be oveloaded with how node behaves at resume.
         */
        virtual void resume() = 0;

        /**
         *	@brief	Pure virtual update method.
         *  @details Needs to be oveloaded with how node behaves at update which basicaly means every frame..
         */
        virtual void update() = 0;

        /**
         *	@brief	Pure virtual finish method.
         *  @details Needs to be oveloaded with how node behaves at finish. That is when node finished it's job or for ex game quit.
         */
        virtual void finish() = 0;

        /**
         *	@brief	True if node finished it's work.
         */
        bool m_done;

        /**
         *	@brief	Node priority.
         */
        uint32_t m_priority;
        friend class ALoop;
    };

    typedef aex::shared_ptr<ALoopNode> ALoopNodePtr;
}
#endif /* defined(__aexolGL__ALoopNode__) */
