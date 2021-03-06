/*
 * Copyright (c) 2013 Hrvoje Zeba <zeba.hrvoje@gmail.com>
 *
 *    This file is part of dagger.
 *
 *    dagger is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once


#include <dagger/types.h>


namespace dagger {


class channel
{
public:
    static const int32_t max_value = 99999999;

public:
    class different_channels_error : public std::runtime_error
    {
    public:
        different_channels_error()
          : runtime_error("channels don't have same dimensions")
        {
        }
    };

    class invalid_alpha_channel_error : public std::runtime_error
    {
    public:
        invalid_alpha_channel_error()
          : runtime_error("alpha channel not compatible data")
        {
        }
    };

public:
    channel()
      : m_width(0)
      , m_height(0)
      , m_image_size(0)
    {
    }

    channel(uint16_t width, uint16_t height)
    {
        create_channel(width, height);
        std::fill_n(m_data.get(), m_image_size, 0);
    }

public:
    bool empty() const
    {
        if (m_image_size == 0)
            return true;

        return false;
    }

    int16_t width() const
    {
        return m_width;
    }

    int16_t height() const
    {
        return m_height;
    }

    int32_t image_size() const
    {
        return m_image_size;
    }

    cdata_t data() const
    {
        return m_data;
    }

    data_t data()
    {
        return m_data;
    }

    static bool equal_dimensions(const channel& c1, const channel& c2)
    {
        if (c1.m_width != c2.m_width)
            return false;

        if (c1.m_height != c2.m_height)
            return false;

        return true;
    }

    void set_value(int16_t x, int16_t y, int32_t value)
    {
        assert(x >= 0 && x < m_width);
        assert(y >= 0 && y < m_height);
        assert(value >= 0 && value <= max_value);

        int32_t* data = m_data.get();
        data[y * m_width + x] = value;
    }

    int32_t get_value(int16_t x, int16_t y, int32_t default_value) const
    {
        bool use_default = false;

        use_default |= x < 0;
        use_default |= x >= m_width;
        use_default |= y < 0;
        use_default |= y >= m_height;

        if (use_default == true)
            return default_value;

        int32_t* data = m_data.get();
        return data[y * m_width + x];
    }

    int32_t get_value(int16_t x, int16_t y) const
    {
        assert(x >= 0 && x < m_width);
        assert(y >= 0 && y < m_height);

        return get_value(x, y, 0);
    }

    void view(const channel& c, int16_t x, int16_t y) const
    {
        int16_t c_width = c.width();
        int16_t c_height = c.height();

        int32_t* data = m_data.get();
        int32_t offset = 0;

        const int32_t* source_data = c.data().get();

        for (int16_t j = y; j < y + m_height; j++)
        {
            for (int16_t i = x; i < x + m_width; i++)
            {
                int32_t v = 0;

                bool valid_position = true;

                valid_position &= i >= 0 && i < c_width;
                valid_position &= j >= 0 && j < c_height;

                if (valid_position == true)
                    v = source_data[j * c_width + i];

                assert(v >= 0 && v <= max_value);

                data[offset++] = v;
            }
        }
    }

public:
    channel(channel&& other)
    {
        *this = std::move(other);
    }

    channel& operator=(channel&& other)
    {
        if (this == &other)
            return *this;

        m_width = other.m_width;
        other.m_width = 0;

        m_height = other.m_height;
        other.m_height = 0;

        m_image_size = other.m_image_size;
        other.m_image_size = 0;

        m_data = std::move(other.m_data);

        return *this;
    }

    channel(const channel& other)
    {
        *this = other;
    }

    channel& operator=(const channel& other)
    {
        if (this == &other)
            return *this;

        create_channel(other.m_width, other.m_height);
        std::copy_n(other.m_data.get(), m_image_size, m_data.get());

        return *this;
    }

private:
    void create_channel(int16_t width, int16_t height)
    {
        assert(width > 0);
        assert(height > 0);

        m_width = width;
        m_height = height;

        m_image_size = height * width;

        m_data.reset(new int32_t[m_image_size]);
    }

private:
    data_t  m_data;

    int16_t m_width;
    int16_t m_height;

    int32_t m_image_size;
};

}
