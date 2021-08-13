// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_HumanUI.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include "Components/Image.h"
#include "Components/Button.h"
#include "Engine/World.h"
#include "SpaceWarGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UPlayer_HumanUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!Owner.IsValid())
		return;

	HpBar_1->SetPercent(Owner->GetPlayerHP() / Owner->GetPlayerFullHP());
	MPBar->SetPercent(Owner->GetPlayerMP() / Owner->GetPlayerFullMP());

	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);

	currentMagazine->SetText(FText::AsNumber(Owner->GetBulletCount(), &Opts));
	maxMagazine->SetText(FText::AsNumber(Owner->GetMaxBulletCount(), &Opts));
	currentHP->SetText(FText::AsNumber(Owner->GetPlayerHP(), &Opts));
	fullHP->SetText(FText::AsNumber(Owner->GetPlayerFullHP(), &Opts));

	Pistol->SetPercent(Owner->GetPlayerMP() / 50);
	PistolBG->SetPercent(Owner->GetPlayerMP() / 50);
	Spike->SetPercent(Owner->GetPlayerMP() / 100);
	SpikeBG->SetPercent(Owner->GetPlayerMP() / 100);

}


void UPlayer_HumanUI::GamePauseUIVisible(bool OnOff)
{
	if (!OnOff)
	{
		playBtn->SetVisibility(ESlateVisibility::Hidden);
		pauseBG->SetVisibility(ESlateVisibility::Hidden);
		tutorialBtn->SetVisibility(ESlateVisibility::Hidden);
		quitBtn->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		playBtn->SetVisibility(ESlateVisibility::Visible);
		pauseBG->SetVisibility(ESlateVisibility::Visible);
		tutorialBtn->SetVisibility(ESlateVisibility::Visible);
		quitBtn->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlayer_HumanUI::GamePlayBtn_Clicked()
{
	Owner->GamePause();
}

void UPlayer_HumanUI::TutorialBtn_Clicked()
{
	Owner->TutorialBegin();
}

void UPlayer_HumanUI::QuitBtn_Clicked()
{	
	Cast<ASpaceWarGameModeBase>(GetWorld()->GetAuthGameMode())->SaveData_Human();
	//게임 종료는 그냥 blueprint내에서함
}