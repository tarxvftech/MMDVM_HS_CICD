/*
 *   Copyright (C) 2009-2018,2020,2021 by Jonathan Naylor G4KLX
 *   Copyright (C) 2017 by Andy Uribe CA6JAU
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Config.h"
#include "Globals.h"
#include "M17TX.h"

#include "M17Defines.h"

const uint8_t M17_SYNC = 0x77U;
const uint8_t M17_PREAMBLE = 0x77U;

CM17TX::CM17TX() :
m_buffer(1500U),
m_poBuffer(),
m_poLen(0U),
m_poPtr(0U),
m_txDelay(240U),      // 200ms
m_delay(false)
{
}

void CM17TX::process()
{
  if (m_buffer.getData() == 0U && m_poLen == 0U)
    return;

  if (m_poLen == 0U) {
    if (!m_tx) {
      m_delay = true;
      m_poLen = m_txDelay;
    } else {
      m_delay = false;
      for (uint8_t i = 0U; i < M17_FRAME_LENGTH_BYTES; i++)
        m_poBuffer[m_poLen++] = m_buffer.get();
    }

    m_poPtr = 0U;
  }

  if (m_poLen > 0U) {
    uint16_t space = io.getSpace();
    
    while (space > 8U) {
      if (m_delay) {
        writeByte(M17_SYNC);
        m_poPtr++;
      } else
        writeByte(m_poBuffer[m_poPtr++]); 

      space -= 8U;
      
      if (m_poPtr >= m_poLen) {
        if (m_delay) {
          m_delay = false;
          m_poPtr = 0U;
          m_poLen = 3U;
        } else {
          m_poPtr = 0U;
          m_poLen = 0U;
          m_delay = false;
          return;
        }
      }
    }
  }
}

uint8_t CM17TX::writeData(const uint8_t* data, uint8_t length)
{
  if (length != (M17_FRAME_LENGTH_BYTES + 1U))
    return 4U;

  uint16_t space = m_buffer.getSpace();
  if (space < M17_FRAME_LENGTH_BYTES)
    return 5U;

  for (uint8_t i = 0U; i < M17_FRAME_LENGTH_BYTES; i++)
    m_buffer.put(data[i + 1U]);

  return 0U;
}

void CM17TX::writeByte(uint8_t c)
{
  uint8_t bit;
  uint8_t mask = 0x80U;
  
  for (uint8_t i = 0U; i < 8U; i++, c <<= 1) {
    if ((c & mask) == mask)
      bit = 1U;
    else
      bit = 0U;

    io.write(&bit, 1);
  }
}

void CM17TX::setTXDelay(uint8_t delay)
{
  m_txDelay = 600U + uint16_t(delay) * 12U;        // 500ms + tx delay
}

uint8_t CM17TX::getSpace() const
{
  return m_buffer.getSpace() / M17_FRAME_LENGTH_BYTES;
}

