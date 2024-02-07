#pragma once

#include "EngineMinimal.h"
#include "EngineGlobals.h"
#include "Engine.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "LoginHeader.h"
#include "GameHeader.h"
#include "NiagaraComponent.h"
#include "Engine/DataTable.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h" //길찾기 헤더

#define	LOGIN_PORT	6380
#define	LOGINSERVER_IP	TEXT("127.0.0.1")

#define	GAMESERVER_IP	TEXT("127.0.0.1")
#define	GAMESERVER_PORT	6381

#define	ROOMSERVER_IP	TEXT("127.0.0.1")
#define	ROOMSERVER_PORT	6382

#define	CHATTINGSERVER_IP	TEXT("127.0.0.1")
#define	CHATTINGSERVER_PORT	6383

#define	MOVESERVER_IP	TEXT("127.0.0.1")
#define	MOVESERVER_PORT	6384

#define	PACKET_LENGTH	4096
#define MESSAGE_LENGTH  128
#define USERNAME_SIZE	64
#define	JOBNAME_SIZE	64
#define	ITEMNAME_SIZE	32
#define INVENTORY_MAX	30
#define SKILLNAME_SIZE	64
#define QUESTNAME_SIZE	64
#define DESC_SIZE		512
#define CHATTING_SIZE	20
#define CREATECHARACTERCOUNT 3


UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	Minion,
	Golem,
	Troll,
	Boss,
	FinalBoss,
	Sevarog
};

UENUM(BlueprintType)
enum class ENPCType : uint8
{
	Store,
	SevarogRoomCreate
};

typedef struct _tagPacket
{
	int32	iLength;
	int32	iNumber;
	int32	iProtocol;
	uint8	packet[PACKET_LENGTH];

	_tagPacket()
	{
		FMemory::Memset(packet, 0, sizeof(uint8) * PACKET_LENGTH);
	}
}Packet, *PPacket;

typedef struct _tagRecvQueueData
{
	int32	iProtocol;
	int32	iLength;
	uint8	packet[PACKET_LENGTH];

	_tagRecvQueueData()
	{
		iProtocol = 0;
		iLength = 0;
		FMemory::Memset(packet, 0, sizeof(uint8) * PACKET_LENGTH);
	}
}RecvQueueData, *PRecvQueueData;


DECLARE_LOG_CATEGORY_EXTERN(LDH, Log, All);
#define	LOG_CALLINFO		(FString(__FUNCTION__) + TEXT("{") + FString::FromInt(__LINE__) + TEXT("}"))
#define	LOG(Format, ...)	UE_LOG(LDH, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

void PrintViewport(float fTime, const FColor& color, const FString& strText);

void ConvertString(char* pResult, const FString& str);
void ConvertString(FString& strResult, const char* pStr);

enum class MOVE_TYPE	: uint8
{
	None,
	MoveStart,
	Move,
	MoveEnd
};
enum class ATTACK_TYPE : uint8
{
	None,
	NormalAttack,
	Skill1,
	Skill2,
	Skill3,
	Skill4
};

UENUM(BlueprintType)
enum class MonsterAnim : uint8
{
	Idle,
	Walk,
	Run,
	Attack,
	Death,
	Skill_1,
	Skill_2
};

UENUM(BlueprintType)
enum class EItemGrade : uint8
{
	Normal,
	Rare,
	Epic,
	Unique,
	Legendary
};

UENUM(BlueprintType)
enum class ItemType : uint8
{
	IT_WEAPON,
	IT_ARMOR,
	IT_HELMET,
	IT_SHIELD,
	IT_POTION,
	IT_END
};

UENUM(BlueprintType)
enum ItemOptionType
{
	IOT_ATTACK,
	IOT_ARMOR,
	IOT_HP,
	IOT_MP,
	IOT_HPRECOVERY,
	IOT_MPRECOVERY,
	IOT_CRITICALRATIO,
	IOT_CRITICALDAMAGE,
	IOT_MOVESPEED,
	IOT_END
};

struct FItemOption
{
	ItemOptionType	Type;
	int32		Option;
};

struct FItemInfoData
{
	char	strName[ITEMNAME_SIZE];
	EItemGrade Grade;
	ItemType	Type;
	int		Buy;
	int		Sell;
	TArray<FItemOption>  tOption;
};

UENUM(BlueprintType)
enum class ELevel : uint8
{
	None,
	Kingdom,
	Desert,
	SevarogBossRoom
};


struct _tagCharacterInfo
{
	int32	iMapLevel;
	//----------------------------
	char	strJob[JOBNAME_SIZE];
	char	strName[USERNAME_SIZE];
	int32		iAttack;
	int32		iArmor;
	float	fAttackSpeed;
	float	fMoveSpeed;
	float	fCritical;
	float	fCriticalDamage;
	int32		iHP;
	int32		iHPMax;
	int32		iMP;
	int32		iMPMax;
	int32		iLevel;
	int32		iExp;
	//unsigned __int64		iExpMax;
	int32		iExpMax;
	int32		iMoney;
	int32		iSkillPoint;
};

struct _tagCharacterInfoTable
{
	int32	iCharacterNumber;
	int32	iCharacterTableNumber;
	int32	iUserNumber;

	//----------------------------
	int32	iMapLevel;
	char	strJob[JOBNAME_SIZE];
	char	strName[USERNAME_SIZE];
	int32	iAttack;
	int32	iArmor;
	float	fAttackSpeed;
	float	fMoveSpeed;
	float	fCritical;
	float	fCriticalDamage;
	int32	iHP;
	int32	iHPMax;
	int32	iMP;
	int32	iMPMax;
	int32	iLevel;
	int32	iExp;
	//unsigned __int64		iExpMax;
	int32	iExpMax;
	int32	iMoney;
	int32	iSkillPoint;
};

UENUM(BlueprintType)
enum class EPlayerAnimationType : uint8
{
	Idle,
	Run,
	Attack,
	Run_Attack,
	Death,
	Hit,
	Skill1,
	Skill2,
	Skill3
};

//-------------------------------------------------------------
UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Passive,
	Active
};

UENUM(BlueprintType)
enum class EJob : uint8
{
	GreyStone,
	TwinBlast,
	Serath,
	Modekaiser,
	End
};

UENUM(BlueprintType)
enum class ESkillProperty : uint8
{
	Physical, // 물리
	Magical, // 마법 Psychic도 포함
	Fire, // 화염
	Frozen, // 냉기
	Lightning, // 번개
	Posion, // 독 Plagued 역병
	Divine, // 신성
};

UENUM(BlueprintType)
enum class ESkillOptionType : uint8
{
	Damage, // 정해진수치
	PercentDamage, // 추가데미지
	AugmentPropt, // 속성부여

	ATKUp,
	DEFUp,
	ATKDown,
	DEFDown,

	Heals,
	PercentHeals,
	EnhancesHP, //  일시적으로 Max체력 증가
	EnhancesMP,//
	HPDown, //
	HPRecovery,//체력 회복속도
	MPRecovery,//마나 회복속도

	IncreaseMoveSpeed,
	IncreaseATKSpeed,
	SlowDown, // 이속둔화
	Cripple, // 공속둔화

	DamageReturned, //데미지 반사
	DamageImmunity, //피해면역
	PercentLifeSteal,//흡혈(최소수치 정하고) 이 피해를 내 공격력의 % 인지 적 체력의 %인지 결정해야함...

	//DOT 지속피해
	Bleed,//출혈
	Poison,//중독
	Burn,//화상
	Shock,//감전
	Freeze,//빙결

	Stun,//기절
	BlindEnemy, //적 실명 : 적의 모든 데미지 무효
	Weaken, // 저주용(능력치 절반 감소)
	Summon, //소환수 소환 / 터렛같은 설치물도 포함 
	UnSummon, //소환수 해제

	Sneak, //은신 (stealth, DarkSide) 어그로가 안끌린다.
	Taunt, //도발
	Kinematic, // 강제이동효과 끌어당김, 블랙홀같은 마법
	Transform,//변신
	Polymorph, //변이
	Resurrection // 부활
};

UENUM(BlueprintType)
enum class ESkillActionType : uint8
{
	Once,
	Multi,
	Duration,	// 지속시간
	Interval	// 시간간격
};

typedef struct _tagSkillOption
{
	ESkillOptionType	SkillOptionType;
	float				SkillOptionData;
}SkillOption, * PSkillOption;

typedef struct _tagSkillAction
{
	ESkillActionType	SkillActionType;
	float				SkillActionData;
}SkillAction, * PSkillAction;

typedef struct _tagSkillInfo
{
	ESkillType	eType;
	EJob	eJob;
	ESkillProperty ePropt;
	int		iSkillNumber;
	int		iSkillLevelMax;
	int		iRequiredLevel;
	float	fCooldown;
	char	strName[SKILLNAME_SIZE];
	char	strDesc[DESC_SIZE];
	TArray<SkillOption>	vecOption;
	TArray<SkillAction>	vecAction;
	TArray<int>	vecNextSkillNumber;
	TArray<int>	vecPrevSkillNumber;

	_tagSkillInfo()	:
		strName{},
		strDesc{}
	{
	}
}SkillInfo, * PSkillInfo;

typedef struct _tagRectangle
{
	FVector2D	Pos;
	FVector2D	Size;
}Rectangle, *PRectangle;

typedef struct _tagSkillAnimation
{
	UAnimMontage*	Animation;
	int32			SkillNumber;
}SkillAnimation, *PSkillAnimation;


enum class EQuestType : UINT8
{
	QT_COLLECTION,
	QT_HUNTER,
	QT_SPOT
};

struct _tagQuestOption
{
	EQuestType	Type;
	int32		SerialNumber;	// 아이템 수집일 경우 아이템 번호, 사냥일 경우 몬스터 번호
	int32		OptionData;
};

// 퀘스트 정보를 저장하기 위한 데이터베이스를 만들고 거기에 퀘스트 정보를 미리 저장하던가
// 아니면 파일로 퀘스트 목록 파일을 만들고 서버가 시작될 때 퀘스트 정보를 불러온다.
typedef struct _tagQuestInfo
{
	int32		QuestNumber;
	char	strName[QUESTNAME_SIZE];
	char	strDesc[DESC_SIZE];
	_tagQuestOption	Option;
	int32		QuestExp;

	_tagQuestInfo() :
		QuestNumber(0),
		strName{},
		strDesc{},
		Option{},
		QuestExp(0)
	{
	}
}QuestInfo;

enum class ESlotType : uint8
{
	None,
	Skill,
	Item
};
