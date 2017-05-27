#pragma once
#include <cstdint>
#include <vector>


namespace msgpackpp {

	enum pack_type : std::uint8_t {
		NIL = 0xc0,

		FALSE = 0xc2,
		TRUE = 0xc3,
	};

    class packer
    {
        std::vector<std::uint8_t> m_buffer;

    public:
        void pack_nil()
        {
            m_buffer.push_back(pack_type::NIL);
        }

        void pack(bool isTrue)
        {
            if(isTrue){
                m_buffer.push_back(pack_type::TRUE);
            }
            else{
                m_buffer.push_back(pack_type::FALSE);
            }
        }

        const std::vector<std::uint8_t> &get_payload()const { return m_buffer; }
    };

    packer create_packer()
    {
        return packer();
    }

}
