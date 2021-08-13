// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestListWidget.h"
#include "QuestListSlot.h"
#include <Components/TextBlock.h>


void UQuestListWidget::AddList(FQuestStruct questData, uint8 inputQuestNumber)
{		
	if (questSlotFactory)
	{
		currentQuestCount++;
		questSlot.Add(CreateWidget<UQuestListSlot>(GetWorld(), questSlotFactory));	
		questSlot[currentQuestCount]->SetQuestNumber(inputQuestNumber);
		questSlot[currentQuestCount]->UpdateList(questData.questName, FText::AsNumber(questData.RemoveGoal1));
		questSlot[currentQuestCount]->SetRenderTranslation(FVector2D(0,-150 + currentQuestCount*150));			
		if (IsInViewport())	questSlot[currentQuestCount]->AddToViewport();
	}			
}

void UQuestListWidget::RemoveList(int32 inputQuestNumber)
{
	int32 count = 0;
	for (UQuestListSlot* temp : questSlot)
	{
		if (temp->GetQuestNumber() == inputQuestNumber)
		{	
			temp->RemoveFromViewport();
			questSlot.RemoveAt(count);			
			break;
		}
		count++;
	}
	currentQuestCount--;
	ListSort();
}

void UQuestListWidget::SetVisibleQuestList(bool isOnOff)
{
	if (!isOnOff)
	{
		RemoveFromViewport();

		for (UQuestListSlot* temp : questSlot)
		{
			temp->RemoveFromViewport();			
		}		
	}
	else
	{
		AddToViewport();

		for (UQuestListSlot* temp : questSlot)
		{
			temp->AddToViewport();			
		}		
	}
}

void UQuestListWidget::ListSort()
{
	int32 count = 0;
	for (UQuestListSlot* temp : questSlot)
	{
		
		temp->SetRenderTranslation(FVector2D(0, -150 + count
			* 150));
					
		count++;
	}
}