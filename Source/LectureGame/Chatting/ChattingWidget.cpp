// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/ListView.h"
#include "Components/Button.h"
#include "../ChattingServerManager.h"
#include "ChatData.h"
#include "../NetStream.h"
#include "../NetSession.h"
#include "../PacketSession.h"
#include "../NetworkManager.h"

void UChattingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ChatInput = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ChatInput")));
	ChatList = Cast<UListView>(GetWidgetFromName(TEXT("ChatList")));
	SendButton = Cast<UButton>(GetWidgetFromName(TEXT("SendButton")));

	ChatInput->OnTextCommitted.AddDynamic(this, &UChattingWidget::ChatCommit);
	SendButton->OnClicked.AddDynamic(this, &UChattingWidget::SendButtonClick);
}

void UChattingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}

void UChattingWidget::AddChatting(FString Text, bool Notice)
{
	UChatData* pData = NewObject<UChatData>(this, UChatData::StaticClass());

	pData->SetText(Text);
	pData->SetNotice(Notice);

	ChatList->AddItem(pData);
}

void UChattingWidget::ChatCommit(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::Type::OnEnter)
	{
		// Enter키를 입력했을때 채팅을 서버로 전송해준다.
		SendChatText();
	}
}

void UChattingWidget::SendButtonClick()
{
	SendChatText();
}

void UChattingWidget::SendChatText()
{
	PacketSession* ChattingSession = ChattingServerManager::GetInst()->GetSession();
	char SendChatInput[MESSAGE_LENGTH] = {};
	int32 SendChatSize = 0;
	FString MyUserName = NetworkManager::GetInst()->GetName();
	if (ChatInput)
	{
		NetStream	stream;
		uint8	packet[PACKET_LENGTH] = {};
		stream.SetBuffer(packet);
		SendChatSize = MESSAGE_LENGTH;
		
		//FMemory::Memcpy(SendChatInput, TCHAR_TO_ANSI(*ChatInput->GetText().ToString()), MESSAGE_LENGTH);
		//char* Message = TCHAR_TO_ANSI(*Test);

		FString Test = ChatInput->GetText().ToString();

		const char* result = TCHAR_TO_UTF8(*Test);

		int32 iSize = strnlen(result, MESSAGE_LENGTH);
		FMemory::Memcpy(SendChatInput, result, iSize);

		stream.Write(&iSize, sizeof(SendChatSize));
		stream.Write(SendChatInput, iSize);

		ChattingSession->Write(CP_MESSAGE, stream.GetLength(), packet);
	}

	if (ChatList->GetNumItems() > CHATTING_SIZE)
	{
		UChatData* RemoveData = Cast<UChatData>(ChatList->GetItemAt(0));
		ChatList->RemoveItem(RemoveData);
	}

	{
		UChatData* pData = NewObject<UChatData>(this, UChatData::StaticClass());
		pData->SetText(MyUserName + TEXT(" : ") + ChatInput->GetText().ToString());
		ChatList->AddItem(pData);
		ChatInput->SetText(FText());
	}
}
