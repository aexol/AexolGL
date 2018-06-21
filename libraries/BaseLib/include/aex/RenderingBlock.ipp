#ifndef __AEX_RENDERINGBLOCK_IPP
#define __AEX_RENDERINGBLOCK_IPP
#include <aex/Common.hpp>
#include "RenderingBlock.hpp"
namespace aex
{
    namespace __renderingblock_detail
    {
        template<class... Args>
        struct block_runner;

        template<class BlockTypeT, class BlockTraitsT, class... BlockList>
        struct block_runner<RenderingBlock<BlockTypeT, BlockTraitsT>, BlockList...>
        {
            void operator()(RenderingBlock<BlockTypeT, BlockTraitsT>& rb, BlockList&... list)
            {
                rb.beginFrame();
                block_runner<BlockList...>{}(list...);
                rb.endFrame();
            }
        };

        template<>
        struct block_runner<>
        {
            void operator()(){}
        };
        template<class... Args>
        struct block_initializer;

        template<class BlockTypeT, class BlockTraitsT, class... BlockList>
        struct block_initializer<RenderingBlock<BlockTypeT, BlockTraitsT>, BlockList...>
        {
            bool operator()(RenderingBlock<BlockTypeT, BlockTraitsT>& rb, BlockList&... list)
            {
                return rb.init() & block_initializer<BlockList...>{}(list...);
            }
        };

        template<>
        struct block_initializer<>
        {
            bool operator()(){return true;}
        };
    }

    template<class BlockTypeT>
    bool
    BlockTraits<BlockTypeT>::init(BlockTypeT& block)
    {
        return block.init();
    }

    template<class BlockTypeT>
    void
    BlockTraits<BlockTypeT>::beginFrame(BlockTypeT& block)
    {
        block.beginFrame();
    }

    template<class BlockTypeT>
    void
    BlockTraits<BlockTypeT>::endFrame(BlockTypeT& block)
    {
        block.endFrame();
    }

    template<class BlockTypeT>
    void
    BlockTraits<BlockTypeT>::pause(BlockTypeT& block)
    {
        block.pause();
    }

    template<class BlockTypeT>
    void
    BlockTraits<BlockTypeT>::resume(BlockTypeT& block)
    {
        block.resume();
    }

    template<class BlockTypeT>
    void
    RunInitOnlyBlock<BlockTypeT>::pause(BlockTypeT& block)
    {
    }

    template<class BlockTypeT>
    void
    RunInitOnlyBlock<BlockTypeT>::resume(BlockTypeT& block)
    {
    }

    template<class BlockTypeT>
    bool
    RunInitOnlyBlock<BlockTypeT>::init(BlockTypeT& block)
    {
        return block.init();
    }

    template<class BlockTypeT>
    void
    RunInitOnlyBlock<BlockTypeT>::beginFrame(BlockTypeT& block)
    {
        block.beginFrame();
    }

    template<class BlockTypeT>
    void
    RunInitOnlyBlock<BlockTypeT>::endFrame(BlockTypeT& block)
    {
        block.endFrame();
    }

    template<class BlockTypeT>
    void
    RunOnlyBlock<BlockTypeT>::beginFrame(BlockTypeT& block)
    {
        block.beginFrame();
    }

    template<class BlockTypeT>
    void
    RunOnlyBlock<BlockTypeT>::endFrame(BlockTypeT& block)
    {
        block.endFrame();
    }

    template<class BlockTypeT>
    bool
    RunOnlyBlock<BlockTypeT>::init(BlockTypeT& block)
    {
        return true;
    }

	template<class BlockTypeT, class BlockTraitsT>
	RenderingBlock<BlockTypeT, BlockTraitsT>::RenderingBlock(const BlockTypeT& block) : m_block{block}
	{
        
	}

	template<class BlockTypeT, class BlockTraitsT>
    RenderingBlock<BlockTypeT, BlockTraitsT>::RenderingBlock(BlockTypeT&& block) : m_block{std::move(block)}
	{

	}

    template<class BlockTypeT, class BlockTraitsT>
    template<class... Args>
    RenderingBlock<BlockTypeT, BlockTraitsT>::RenderingBlock(Args&&... args) : m_block{aex::forward<Args>(args)...}{}

	template<class BlockTypeT, class BlockTraitsT>
	void
	RenderingBlock<BlockTypeT, BlockTraitsT>::setBlock(const BlockTypeT& block)
	{
        m_block = block;
	}

	template<class BlockTypeT, class BlockTraitsT>
	void
	RenderingBlock<BlockTypeT, BlockTraitsT>::setBlock(BlockTypeT&& block)
	{
        m_block = std::move(block);
	}

	template<class BlockTypeT, class BlockTraitsT>
	bool
	RenderingBlock<BlockTypeT, BlockTraitsT>::init()
	{
        return BlockTraitsT::init(m_block);
	}

	template<class BlockTypeT, class BlockTraitsT>
	void
	RenderingBlock<BlockTypeT, BlockTraitsT>::pause()
	{
        BlockTraitsT::pause(m_block);
	}

	template<class BlockTypeT, class BlockTraitsT>
	void
	RenderingBlock<BlockTypeT, BlockTraitsT>::resume()
	{
        BlockTraitsT::resume(m_block);
	}

	template<class BlockTypeT, class BlockTraitsT>
	void
	RenderingBlock<BlockTypeT, BlockTraitsT>::beginFrame()
	{
        BlockTraitsT::beginFrame(m_block);
	}

	template<class BlockTypeT, class BlockTraitsT>
	void
	RenderingBlock<BlockTypeT, BlockTraitsT>::endFrame()
	{
        BlockTraitsT::endFrame(m_block);
	}

    template<class BlockTypeT, class BlockTraitsT>
    BlockTypeT&
    RenderingBlock<BlockTypeT, BlockTraitsT>::getBlock()
    {
        return m_block;
    }

    template<class BlockTypeT, class BlockTraitsT>
    const BlockTypeT&
    RenderingBlock<BlockTypeT, BlockTraitsT>::getBlock() const
    {
        return m_block;
    }

    template<class BlockTypeT, class BlockTraitsT>
    BlockTypeT*
    RenderingBlock<BlockTypeT, BlockTraitsT>::operator->()
    {
        return &m_block;
    }

    template<class BlockTypeT, class BlockTraitsT>
    const BlockTypeT*
    RenderingBlock<BlockTypeT, BlockTraitsT>::operator->() const
    {
        return &m_block;
    }

    template</*class BlockTypeT, class BlockTraitsT,*/ class... BlockList>
    void runRenderingBlocks(/*RenderingBlock<BlockTypeT, BlockTraitsT>& block,*/ BlockList&... blocks)
    {
        //block.beginFrame();
        //runRenderingBlocks(aex::forward<BlockList>(blocks)...);
        //block.endFrame();
        __renderingblock_detail::block_runner<BlockList...>{}(blocks...);
    }

    template</*class BlockTypeT, class BlockTraitsT,*/ class... BlockList>
    bool initRenderingBlocks(/*RenderingBlock<BlockTypeT, BlockTraitsT>& block,*/ BlockList&... blocks)
    {
        //return block.init() && initRenderingBlocks(aex::forward<BlockList>(blocks)...);
        return __renderingblock_detail::block_initializer<BlockList...>{}(blocks...);
    }
} // end of aex
#endif  // __AEX_RENDERINGBLOCK_IPP
