#pragma once

//-------------------------
//		1차시도 싱글스레드
//-------------------------

//struct SListEntry
//{
//	SListEntry* next;
//
//};
//
//struct SListHeader
//{
//	SListEntry* next = nullptr;
//};
//
//// [data][     ][ ][ ]
//// [Header]
//
//void InitiallizeHead(SListHeader* header);
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//SListEntry* PopEntrySList(SListHeader* header);


//-------------------------
//		2차시도 멀티스레드
//-------------------------

//struct SListEntry
//{
//	SListEntry* next;
//
//};
//
//struct SListHeader
//{
//	SListEntry* next = nullptr;
//};
//
//// [data][     ][ ][ ]
//// [Header]
//
//void InitiallizeHead(SListHeader* header);
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//SListEntry* PopEntrySList(SListHeader* header);

//-------------------------
//		3차시도 멀티스레드 aba ploblem 해결
//-------------------------

DECLSPEC_ALIGN(16)
struct SListEntry
{
	SListEntry* next;

};

DECLSPEC_ALIGN(16)
struct SListHeader
{
	SListHeader()
	{
		alignment = 0;
		region = 0;
	}

	union 
	{
		struct
		{
			uint64 alignment;
			uint64 region;
		} DUMMYSTRUCTNAME;
		struct
		{
			uint64 depth : 16;
			uint64 sequence : 48;
			uint64 reserved : 4;
			uint64 next : 60;
		} HeaderX64;
	};

	SListEntry* next = nullptr;
};

void InitiallizeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);