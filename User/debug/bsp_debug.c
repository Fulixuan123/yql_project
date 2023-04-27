#include "bsp_debug.h"
#include "usbd_cdc_if.h"

extern float g_pidSpeedPara[6];
extern float g_pidPosPara[6];

/*************************************** 锟斤拷一锟斤拷锟斤拷 CRC校锟斤拷 ********************************************/
// send_data M1;                           //锟斤拷锟�1锟斤拷要锟矫碉拷锟侥憋拷锟斤拷
/* CRC 锟斤拷位锟街斤拷值锟斤拷 */
static const uint8_t s_crchi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* CRC 锟斤拷位锟街斤拷值锟斤拷 */
const uint8_t s_crclo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};


uint16_t crc16_calc(uint8_t *_pBuf, uint16_t _usLen)
{
    uint8_t ucCRCHi = 0xFF;
    uint8_t ucCRCLo = 0xFF;
    uint16_t usIndex;

    while (_usLen--)
    {
        usIndex = ucCRCLo ^ *_pBuf++;
        ucCRCLo = ucCRCHi ^ s_crchi[usIndex];
        ucCRCHi = s_crclo[usIndex];
    }
    return ((uint16_t)ucCRCHi << 8 | ucCRCLo);
}


void debug_obj_init(send_data *data)
{
    size_t obj_size = sizeof(send_data) * M_NUM;
    memset(data, 0, (size_t)obj_size);
}


extern	uint8_t receive[RX_LEN];
extern  uint8_t receive_ip;

receive_data R_M[M_NUM];

send_data S_M[M_NUM];
//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
void debug_handle(void)
{
	uint8_t temp[RX_LEN] = {0};
	uint8_t i = 0;
	uint8_t type = 0;;

	if (receive[receive_ip-1] == FRAME_END)
	{
	///////////////////////////////////////////////////////////////////////////////////////////////////
		if (receive[(receive_ip + RX_LEN - 8) % RX_LEN] == FRAME_HEAD)
		{
			 for (i = 0; i < 5; i++)
			 {
				temp[i] = receive[(receive_ip + RX_LEN - 8 + i) % RX_LEN];
			 }
		
#if EN_CRC
			 if (crc16_calc(temp, 5) == ((receive[(receive_ip + RX_LEN - 3) % RX_LEN] << 8) | \
										 receive[(receive_ip + RX_LEN - 2) % RX_LEN]))
#endif
			{
				switch (receive[(receive_ip + RX_LEN - 8 + 2) % RX_LEN])
				{
					case R_STATUS:
						type = receive[(receive_ip + RX_LEN - 8 + 1) % RX_LEN] - 1;
						if(type < 0) return;
						R_M[type].R_status = (receive[(receive_ip + RX_LEN - 8 + 4) % RX_LEN]);
						break;
				}
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////
		if (receive[(receive_ip+ RX_LEN - 9) % RX_LEN] == FRAME_HEAD)
		{
			 for (i = 0; i < 6; i++)
			{
				 temp[i] = receive[(receive_ip + RX_LEN - 9 + i) % RX_LEN];
			}

#if EN_CRC
		if (crc16_calc(temp, 6) == ((receive[(receive_ip + RX_LEN - 3) % RX_LEN] << 8) | \
			receive[(receive_ip + RX_LEN - 2) % RX_LEN]))
#endif
			{
				switch (receive[(receive_ip + RX_LEN - 9 + 2) % RX_LEN])
				{
					case R_SPEED:
						type = receive[(receive_ip + RX_LEN - 9 + 1) % RX_LEN] - 1;
						if(type < 0) return;
						R_M[type].R_speed = (int16_t)((receive[(receive_ip + RX_LEN - 9 + 4) % RX_LEN] << 8)| \
													  receive[(receive_ip + RX_LEN - 9 + 5) % RX_LEN]);
					break;
				}
			}
		 }
		////////////////////////////////////////////////////////////////////////////////////////////////
		if (receive[(receive_ip+ RX_LEN - 11) % RX_LEN] == FRAME_HEAD)
		{
			 for (i = 0; i < 8; i++)
			{
				 temp[i] = receive[(receive_ip + RX_LEN - 11 + i) % RX_LEN];
			}

#if EN_CRC
		if (crc16_calc(temp, 8) == ((receive[(receive_ip + RX_LEN - 3) % RX_LEN] << 8) | \
										 receive[(receive_ip + RX_LEN - 2) % RX_LEN]))
#endif
			{
				switch (receive[(receive_ip + RX_LEN - 11 + 2) % RX_LEN])
					{
						case R_INC_POSITIONING:
							type = receive[(receive_ip + RX_LEN - 11 + 1) % RX_LEN] - 1;
							if(type < 0) return;
							R_M[type].R_length = (uint32_t)((receive[(receive_ip + RX_LEN - 11 + 4) % RX_LEN] << 24) | \
										 (receive[(receive_ip + RX_LEN - 11 + 5) % RX_LEN] << 16) |  \
											 (receive[(receive_ip + RX_LEN - 11 + 6) % RX_LEN] <<  8) | \
											 (receive[(receive_ip + RX_LEN - 11 + 7) % RX_LEN] ));
							break;
						case R_ABS_POSITIONING:
							type = receive[(receive_ip + RX_LEN - 11 + 1) % RX_LEN] - 1;
							if(type < 0) return;
							R_M[type].R_abslength = (uint32_t)((receive[(receive_ip + RX_LEN - 11 + 4) % RX_LEN] << 24) | \
													 (receive[(receive_ip + RX_LEN - 11 + 5) % RX_LEN] << 16) |  \
														 (receive[(receive_ip + RX_LEN - 11 + 6) % RX_LEN] <<  8) | \
														 (receive[(receive_ip + RX_LEN - 11 + 7) % RX_LEN] ));
							break;
						}
		  }
		}

		//set pid para
		if (receive[(receive_ip+ RX_LEN - 11) % RX_LEN] == FRAME_HEAD)
		{

			{
				switch (receive[(receive_ip + RX_LEN - 11 + 1) % RX_LEN])
					{
						case 0x01:
							for (int j=0; j<3; j++) {
								g_pidSpeedPara[j] = receive[(receive_ip + RX_LEN - 11 + 2 +j) % RX_LEN];
								g_pidPosPara[j] = receive[(receive_ip + RX_LEN - 11 + 5 +j) % RX_LEN];
							}
							break;
						case 0x02:
							for (int j=0; j<3; j++) {
								g_pidSpeedPara[j + 3] = receive[(receive_ip + RX_LEN - 11 + 2 +j) % RX_LEN];
								g_pidPosPara[j + 3] = receive[(receive_ip + RX_LEN - 11 + 5 +j) % RX_LEN];
							}
							break;
						}
		  }
		}
	}
}
			

			
send_data S_data[M_NUM];
void debug_upload_data(send_data *data, uint8_t upload_type, uint8_t type)
{
    uint8_t cur_data;
    uint8_t upload_data[20];
    upload_data[0] = FRAME_HEAD;
    upload_data[1] = type + 1;
    cur_data = 4;

    switch (upload_type)
    {
        case S_STATUS:
            upload_data[2] = upload_type;
            upload_data[3] = 0x01;
            upload_data[cur_data++] = data->S_status;
            break;

        case S_SPEED:
            upload_data[2] = upload_type;
            upload_data[3] = 0x02;
            upload_data[cur_data++] = (data->S_speed >> 8) & 0xFF;
            upload_data[cur_data++] = data->S_speed & 0xFF;
            break;

        case S_POS:
            upload_data[2] = upload_type;
            upload_data[3] = 0x08;
            upload_data[cur_data++] = (data->S_position >> 56) & 0xFF;     
				    upload_data[cur_data++] = (data->S_position >> 48) & 0xFF;     
				    upload_data[cur_data++] = (data->S_position >> 40) & 0xFF;    
				    upload_data[cur_data++] = (data->S_position >> 32) & 0xFF;     
					  upload_data[cur_data++] = (data->S_position >> 24) & 0xFF;     
				    upload_data[cur_data++] = (data->S_position >> 16) & 0xFF;     
				    upload_data[cur_data++] = (data->S_position >>  8) & 0xFF;     
            upload_data[cur_data++] = (data->S_position) & 0xFF;
            break;

        case S_ERR:
            upload_data[2] = upload_type;
            upload_data[3] = 0x01;
            upload_data[cur_data++] = 0x01;
            break;

        default :
            upload_data[2] = 0xFE;
            break;
    }

    if (upload_data[2] == 0xFE)
    {
        return;
    }
    else
    {
        uint16_t crc_res = crc16_calc(&(upload_data[0]), cur_data);
        upload_data[cur_data++] = (crc_res >> 8) & 0xFF;
        upload_data[cur_data++] = (crc_res) & 0xFF;
        upload_data[cur_data++] = FRAME_END;
        CDC_Transmit_FS(upload_data,cur_data);

    }
}



void debug_init(void)
{
    debug_obj_init(S_data);
}

void debug_send_state(uint8_t state, uint8_t type)
{
    S_data[type].S_status = state;
    debug_upload_data(&S_data[type], S_STATUS, type);
}
 

void debug_send_speed(uint16_t speed, uint8_t type)
{
    S_data[type].S_speed = speed;
    debug_upload_data(&S_data[type], S_SPEED, type);
}


void debug_send_pos(int64_t position, uint8_t type)
{
    S_data[type].S_position = position;
    debug_upload_data(&S_data[type],S_POS, type);
}



void debug_send_err(uint8_t err, uint8_t type)
{
    S_data[type].S_err = err;
    debug_upload_data(&S_data[type], S_ERR, type);
}


uint8_t GetTargetStatus(uint8_t type)
{
    return R_M[type].R_status;
}


uint16_t GetTargetSpeed(uint8_t type)
{
	return R_M[type].R_speed;
}


uint64_t GetTargetPosition(uint8_t type)
{
	return R_M[type].R_abslength;
}


