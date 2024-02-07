#pragma once

enum CHATTING_PROTOCOL
{
	CP_LOGIN,
	CP_LOGOUT,
	CP_MESSAGE,
};


enum GAME_PROTOCOL
{
	GP_CONNECT,
	GP_DISCONNECT,
	GP_CONNECT_FAIL,

	GP_CREATECHARACTER,
	GP_SELECT_CHARACTERINFO,
	GP_CREATE_CHARACTERINFO,
	GP_DELETE_CHARACTERINFO,

	GP_GAMESTART,
	GP_GAMEIN,	// InGame�� ó�� ����
	GP_LEVELSTART,

	GP_USERAPPEAR,// ���� ���� ���
	GP_USERDISAPPEAR,
	GP_OTHERUSERDISAPPEAR,

	GP_SYNC,
	// Ŭ�󿡼� �޴� �뵵�� ���� �̻��
	GP_MOVESTART,
	GP_MOVE,
	GP_MOVEEND,
	GP_ROTATION,

	GP_OTHERUSERAPPEAR,
	GP_OTHERMOVESTART,
	GP_OTHERMOVE,
	GP_OTHERMOVEEND,
	GP_ATTACK,

	GP_CHARACTERLEVELUP,

	GP_PLAYERATTACKDAMAGE,
	GP_MONSTERCREATE,
	GP_MONSTERAI,
	GP_MONSTERDEATH,
	GP_MONSTERITEMDROP,

	GP_SkillList,
	GP_SkillUp,
	GP_SkillSave,

	GP_STORELIST,
	GP_INVENTORYLIST,
	GP_INVENTORYADD,
	GP_INVENTORYDELETE,
	GP_INVENTORYGOLD,

	GP_QUESTLIST,
	GP_QUESTCOMPLETE,

	GP_TRADEREQUEST,
	GP_TRADEREQUESTACCEPT,
	GP_TRADECANCEL,
	GP_TRADEADD,
	GP_TRADEDELETE,
	GP_TRADEREADY,
	GP_TRADESUCCESS,

	GP_GROUPCREATE,
	GP_GROUPREQUEST,
	GP_GROUPACCEPT,
	GP_GROUPCANCEL,
	GP_GROUPLEAVE,
	GP_GROUPKICK,
	GP_GROUPSUCCESS,

	GP_MAINPORTAL,
	GP_SEVAROGBOSSROOM,

	GP_FINDMONSTERPATH,

	GP_ALIVE
};

enum MonsterAIProtocol
{
	MA_IDLE,

	MA_TRACESTART,
	MA_TRACE,
	MA_TRACEEND,

	MA_PATROLSTART,
	MA_PATROL,
	MA_PATROLEND,

	MA_DAMAGE,

	MA_ATTACK,
	MA_ATTACKEND,

	MA_SKILL1,
	MA_SKILL1END,

	MA_SKILL2,
	MA_SKILL2END
};

enum class ESkillUpMessage
{
	Success,
	MaxLevelFail,
	SkillPointFail,
	RequiredLevelFail,
	LinkedSkillFail
};