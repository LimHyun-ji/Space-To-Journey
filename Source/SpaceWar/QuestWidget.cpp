// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWidget.h"
#include <Components/TextBlock.h>
#include "Components/Button.h"
#include "Player_Human.h"

void UQuestWidget::SettingQuestText(FText inputNPCName, FText inputTitle, FText inputConversation, FText inputGoalText, FText inputGoalCount)
{

	title->SetText(inputTitle);
	conversation->SetText(inputConversation);
	GoalText->SetText(inputGoalText);
	GoalCount->SetText(inputGoalCount);
	NPCName->SetText(inputNPCName);
}

void UQuestWidget::SetWidgetOwner(APlayer_Human* inputOwner)
{
	Owner = inputOwner;	
}

void UQuestWidget::ClickedAccept()
{
	Owner->AcceptQuest();
	Owner->TalkNPC_End();
}

void UQuestWidget::ClickedCancel()
{
	Owner->TalkNPC_End();
}

void UQuestWidget::VisibleAcceptBtn(bool inputOnOff)
{	
	if (!inputOnOff)
	{
		acceptBtn->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		acceptBtn->SetVisibility(ESlateVisibility::Visible);
	}
}

void UQuestWidget::ClickedClear()
{
	Owner->QuestClear();
}

void  UQuestWidget::VisibleClearBtn(bool inputOnOff)
{
	if (!inputOnOff)
	{
		clearBtn->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		clearBtn->SetVisibility(ESlateVisibility::Visible);
	}
}