#ifndef _TLV_H_
#define _TLV_H_

#define TLV_FRAME_SIZE		1024		//����TLV���ݽṹ֡��size		//ע�ⲻ�ܳ��������������ݵ�BUF��SIZE

#define TAG_HH_LSH					0x01			//�Ự��ˮ��			4byte  B
#define TAG_ACK						0x0a			//Ӧ����				2byte  N
#define TAG_APP_VERSION				0x0b			//Ӧ�ó���汾��		8byte  N
#define TAG_PARAM_VERSION			0x0c			//�����汾��			8byte  N
#define TAG_REC_TYPE				0x0d			//��¼����				1byte  B
#define TAG_DEVICE_ID				0x0e			//�ն��豸ID			12byte  B
#define TAG_VERIFY_DATA				0x0f			//��֤����				12byte  B    
#define TAG_DJ_DIR					0x10			//���ݵ����·��		4 byte N   
#define TAG_IF_FEATURE				0x11			//�ն˽ӿ�����			8byte  B
#define TAG_HH_RESULT				0x12			//�Ự���				1byte  B 
#define TAG_HH_TYPE					0x13			//�Ự����				1byte  B
#define TAG_FILE_TYPE				0x14			//�ļ�����				1byte  B
#define TAG_FILE_OFFSET				0x15			//�ļ���ƫ��			4byte  B
#define TAG_DATA_LENGTH				0x16			//���ݳ���				4byte  B
#define TAG_FILE_LIST				0x17			//�ļ��б�				x byte B	//�䳤
#define TAG_DOWNLOAD_COMPLETE_FLAG	0x18			//���ؽ����ı��		1byte  B
#define TAG_TRNAS_DATA				0x19			//͸������				x byte B	//�䳤
#define TAG_OPERATE_CODE			0x60			//������				2byte  B


typedef struct
{
	int						ptr;				//��ǰ�ڰ��ڵ�ָ��λ��
	unsigned char			*buffer;			//�������ָ��
}TTLVPacket;


typedef struct {
	unsigned char			tag;			// tag
	unsigned int			length;			// ��tag�ĳ���
	unsigned char			type;			// tag����������
}TTagInfo;


void tlv_init(TTLVPacket *tlvpacket,unsigned char * databuf);
void tlv_pack(TTLVPacket *tlvpacket);
void tlv_addtag(TTLVPacket *tlvpacket, unsigned char tag, unsigned char *indata);
void tlv_addtag_ext(TTLVPacket *tlvpacket, unsigned char tag,unsigned char *indata,unsigned int len);
int  tlv_verify(unsigned char *indata);
unsigned char *tlv_get_tagvalue(unsigned char tag, TTLVPacket *tlv_packet);
TTagInfo * tlv_get_taginfo(unsigned char tag);
#endif