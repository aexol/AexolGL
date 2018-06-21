#ifndef __AEX_RENDERINGBLOCK_HPP
#define __AEX_RENDERINGBLOCK_HPP
#include <memory>
#include <aex/ALoopNode.hpp>

namespace aex
{
    /**
     * @Brief Default block traits.
     */

    template<class BlockTypeT>
    class BlockTraits
    {
        public:
            /**
             *  @brief Prerender setup.
             */
            static bool init(BlockTypeT& block);

            /**
             * @brief Frame update.
             */
            static void beginFrame(BlockTypeT& block);

            /**
             * @brief Frame update.
             */
            static void endFrame(BlockTypeT& block);

            /**
             * @brief Action on pause.
             */
            static void pause(BlockTypeT& block);

            /**
             *  @brief Action on resume.
             */
            static void resume(BlockTypeT& block);
    };

    template<class BlockTypeT>
    class RunInitOnlyBlock : public BlockTraits<BlockTypeT>
    {
        public:
            static void pause(BlockTypeT& block);
            static void resume(BlockTypeT& block);
            static bool init(BlockTypeT& block);
            static void beginFrame(BlockTypeT& block);
            static void endFrame(BlockTypeT& block);

    };

    /**
     * Only run required from BlockTypeT implementation.
     */
    template<class BlockTypeT>
    class RunOnlyBlock : public RunInitOnlyBlock<BlockTypeT>
    {
        public:
            static bool init(BlockTypeT& block);
            static void beginFrame(BlockTypeT& block);
            static void endFrame(BlockTypeT& block);
    };
    
    /**
     * @brief RenderingBlock.
     *
     * Common interface for all blocks. BlockTypeT must be default constructible, copyable and movable. BlockTraitsT must extend block_traits.
     */
    template<class BlockTypeT, class BlockTraitsT = BlockTraits<BlockTypeT>>
    class RenderingBlock
    {
        static_assert(std::is_convertible<BlockTraitsT*, BlockTraits<BlockTypeT>*>::value, "Invalid template parameter value");
        public:
            typedef BlockTraitsT BlockTraitsType;
            typedef BlockTypeT BlockType;
            /**
             * @brief Constructor
             *
             * Create new rendering tree with block as a new block for tree.
             *
             * @param BlockTypeT Block block.
             */
            RenderingBlock(const BlockTypeT& block);

            /**
             * @brief Constructor
             *
             * Create new rendering tree with block as new block for tree.
             *
             * @param BlockTypeT Block block.
             */
            RenderingBlock(BlockTypeT&& block);

            template<class... Args>
            RenderingBlock(Args&&... args);

            /**
             * @brief Set tree block.
             *
             * Set new block for tree.
             *
             * @param BlockTypeT New block.
             */
            void setBlock(const BlockTypeT& block);

            /**
             * @brief Set tree block.
             *
             * Set new block for tree.
             *
             * @param BlockTypeT New block.
             */
            void setBlock(BlockTypeT&& block);

            /**
             * @brief Init tree.
             *
             * Initialize tree.
             *  
             * @returns bool True if tree sucessfully initialized.
             */
            bool init();

            /**
             * @brief Actions taken on block puase.
             */
            void pause();

            /**
             * @brief Actions taken on tree resume.
             */
            void resume();

            /**
             * @brief Called each frame.
             *
             * Begin drawing block.
             */
            void beginFrame();

            /**
             * @brief Called each frame.
             *
             * End drawing block.
             */
            void endFrame();

            /**
             * @brief Return current block.
             */
            BlockTypeT& getBlock();

            /**
             * @brief Return current block.
             */
            const BlockTypeT& getBlock() const;

            /*
             * @brief Return current block.
             */
            BlockType* operator->();
            const BlockType* operator->() const;
        private:
            BlockTypeT m_block;            
    };

    void runRenderingBlocks();

    /**
     *  @brief Accepts variadic length list of RenderingBlock types. BlockTraitsT::beginFrame() is called for each block. After that BlockTraitsT::endFrame() is called for each block, in order reversed to beginFrame() calls.
     */

    template<class... BlockList>//class BlockTypeT, class BlockTraitsT, class... BlockList>
    void runRenderingBlocks(/* RenderingBlock<BlockTypeT, BlockTraitsT>& block,*/BlockList&... blocks);

    bool initRenderingBlocks();
    /**
     *  @brief Accepts variadic length list of RenderingBlock types. BlockTraitsT::init() is called for each block.
     */
    template</*class BlockTypeT, class BlockTraitsT,*/ class... BlockList>
    bool initRenderingBlocks(/*RenderingBlock<BlockTypeT, BlockTraitsT>& block,*/ BlockList&... blocks);
} // end of aex
#include "RenderingBlock.ipp"
#endif // __AEX_RENDERINGBLOCK_HPP
