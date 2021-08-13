// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHpBar_Widget.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include "Components/Image.h"
#include "Components/Button.h"
#include "Engine/World.h"
#include "SpaceWarGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UPlayerHpBar_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!Owner.IsValid())
		return;

	HpBar->SetPercent(Owner->GetPlayerHP() / Owner->GetPlayerFullHP());
	ReloadBar->SetPercent(Owner->GetPlayerCurrentReloadTime() / Owner->GetPlayerReloadTime());
	MissileCoolTimeBar->SetPercent( Owner->GetCurrentMissileCoolTime() / Owner->GetMissileCoolTime() );

	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);
	currentMagazine->SetText(FText::AsNumber(Owner->GetCurrentMagazine(), &Opts));
	maxMagazine->SetText(FText::AsNumber(Owner->GetMaxMagazine(), &Opts));	
	inclination_Stick_Horizontal->SetRenderTransformAngle(Owner->GetActorRotation().Roll);
	inclination_Stick_Vertical->SetRenderTransformAngle(Owner->GetActorRotation().Pitch);

	if (isMissileCoolTimeDoneOpacityWork) MissileCoolTimeDoneWork();	
	
}


void UPlayerHpBar_Widget::SetMessage(FString inputMessage)
{
	message->SetText(FText::FromString(inputMessage));
}

void UPlayerHpBar_Widget::SetReloadBarVisible(bool inputOnOff)
{
	if(!inputOnOff)	ReloadBar->SetVisibility(ESlateVisibility::Hidden);
	else ReloadBar->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerHpBar_Widget::SetAimingPointVisible(uint8 isAimingPointMode)
{
	if (isAimingPointMode == 0)
	{
		aimingPoint_Normal->SetVisibility(ESlateVisibility::Visible);
		aimingPoint_Speed->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (isAimingPointMode == 1)
	{		
		aimingPoint_Normal->SetVisibility(ESlateVisibility::Hidden);
		aimingPoint_Speed->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{		
		aimingPoint_Normal->SetVisibility(ESlateVisibility::Hidden);
		aimingPoint_Speed->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerHpBar_Widget::MissileCoolTimeDoneWork()
{
	missileCoolTimeDoneScale += 0.05;
	missileCoolTimeDoneOpacity -= 0.05;

	missileCoolTimeDoneImage->SetRenderScale(FVector2D(missileCoolTimeDoneScale, missileCoolTimeDoneScale));
	missileCoolTimeDoneImage->SetRenderOpacity(missileCoolTimeDoneOpacity);

	if (missileCoolTimeDoneOpacity <= 0)	isMissileCoolTimeDoneOpacityWork = false;
	
}

void UPlayerHpBar_Widget::SetMissileCoolTimeDoneWork()
{
	missileCoolTimeDoneScale = 1;
	missileCoolTimeDoneOpacity = 1;
	isMissileCoolTimeDoneOpacityWork = true;
}

void UPlayerHpBar_Widget::GamePauseUIVisible(bool OnOff)
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

void UPlayerHpBar_Widget::GamePlayBtn_Clicked()
{
	Owner->GamePause();
}

void UPlayerHpBar_Widget::TutorialBtn_Clicked()
{
	Owner->TutorialBegin();
}

void UPlayerHpBar_Widget::QuitBtn_Clicked()
{
	Cast<ASpaceWarGameModeBase>(GetWorld()->GetAuthGameMode())->SaveData_SpaceShip();
	//게임 종료는 그냥 blueprint내에서함
}