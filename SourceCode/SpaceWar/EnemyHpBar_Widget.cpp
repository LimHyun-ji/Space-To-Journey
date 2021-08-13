// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHpBar_Widget.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include "SpaceEnemy.h"
#include "Enemy_SpaceGalleon.h"
#include "Enemy_FreeFly.h"
#include "Enemy_Hunter.h"
#include "AllEnemyParent.h"

void UEnemyHpBar_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!OwnerEnemy.IsValid())
		return;
	/*
	if (IsValid(Cast<AEnemy_SpaceGalleon>(OwnerEnemy)))
	{
		AEnemy_SpaceGalleon* Enemy = Cast<AEnemy_SpaceGalleon>(OwnerEnemy);
		HpBar->SetPercent(Enemy->GetCurrentHp() / Enemy->GetMaxHp());
	}
	else if (IsValid(Cast<AEnemy_Hunter>(OwnerEnemy)))
	{
		AEnemy_Hunter* Enemy = Cast<AEnemy_Hunter>(OwnerEnemy);
		HpBar->SetPercent(Enemy->GetCurrentHp() / Enemy->GetMaxHp());
	}
	else if (IsValid(Cast<AEnemy_FreeFly>(OwnerEnemy)))
	{
		AEnemy_FreeFly* Enemy = Cast<AEnemy_FreeFly>(OwnerEnemy);
		HpBar->SetPercent(Enemy->GetCurrentHp() / Enemy->GetMaxHp());
	}
	*/

	HpBar->SetPercent(OwnerEnemy->GetCurrentHp() / OwnerEnemy->GetMaxHp());
	
}
