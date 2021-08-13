// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "PlayerCPP.h"
#include "Player_Human.h"

void UTutorialWidget::SetOwner(class APlayerCPP* inputPlayer)
{
	player = inputPlayer;
	totalPage = TutorialPage.Num();
}

void UTutorialWidget::SetOwner_Human(class APlayer_Human* inputPlayer_Human)
{
	player_Human = inputPlayer_Human;
	totalPage = TutorialPage.Num();
}

void UTutorialWidget::ClickedPreviousBtn()
{
	currentPage--;
	if (currentPage == 0) previousBtn->SetVisibility(ESlateVisibility::Hidden);
	TutorialImage->SetBrushFromTexture(TutorialPage[currentPage]);	

	if (nextBtn->GetVisibility() == ESlateVisibility::Hidden) nextBtn->SetVisibility(ESlateVisibility::Visible);
}

void UTutorialWidget::ClickedNextBtn()
{
	currentPage++;
	if (currentPage == (totalPage-1) ) 	nextBtn->SetVisibility(ESlateVisibility::Hidden);
	TutorialImage->SetBrushFromTexture(TutorialPage[currentPage]);	

	if(previousBtn->GetVisibility() == ESlateVisibility::Hidden) previousBtn->SetVisibility(ESlateVisibility::Visible);
}

void UTutorialWidget::ClickedSkipBtn()
{
	if(IsValid(player))
		player->TutorialEnd();
	else if(IsValid(player_Human))
		player_Human->TutorialEnd();
}

void UTutorialWidget::SetCurrentPage_0()
{
	currentPage = 0;
	
	TutorialImage->SetBrushFromTexture(TutorialPage[currentPage]);
	previousBtn->SetVisibility(ESlateVisibility::Hidden);
	nextBtn->SetVisibility(ESlateVisibility::Visible);
}