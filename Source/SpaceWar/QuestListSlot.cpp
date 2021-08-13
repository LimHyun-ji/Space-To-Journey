// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestListSlot.h"
#include <Components/TextBlock.h>


void UQuestListSlot::UpdateList(FText inputQuestName, FText inputQuestGoal)
{
	questName->SetText(inputQuestName);
	questGoal->SetText(inputQuestGoal);
}

void UQuestListSlot::SetQuestNumber(uint8 inputQuestNumber)
{
	questNumber = inputQuestNumber;
}

uint8 UQuestListSlot::GetQuestNumber()
{
	return questNumber;
}