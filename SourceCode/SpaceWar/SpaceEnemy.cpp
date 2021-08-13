// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include <Components/WidgetComponent.h>
#include "EnemyHpBar_Widget.h"
#include "TargetDisplayWidget.h"
#include "PlayerCPP.h"
#include "Item.h"

// Sets default values
ASpaceEnemy::ASpaceEnemy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	RootComponent = collComp;

	HpBarWidget = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("HpBar"));
	HpBarWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	targetDisplayWidget = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("targetDisplay Widget"));
	targetDisplayWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ASpaceEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	currentHp = maxHp;
	currentMagazine = magazine;

	HpBar = Cast<UEnemyHpBar_Widget>(HpBarWidget->GetUserWidgetObject());
	HpBar->SetOwnerEnemy(this);
	HpBar->SetColorAndOpacity(FLinearColor(200, 0, 0, currentHpBarOpacity));

	targetDisplay = Cast<UTargetDisplayWidget>(targetDisplayWidget->GetUserWidgetObject());

	TArray <AActor*> obj;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCPP::StaticClass(), obj);
	for (AActor* actor : obj)
	{
		target = Cast<APlayerCPP>(actor);

		if (IsValid(target))
			break;
	}

}

// Called every frame
void ASpaceEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ASpaceEnemy::GetCurrentHp()
{
	return currentHp;
}

float ASpaceEnemy::GetMaxHp()
{
	return maxHp;
}


void ASpaceEnemy::SetHpBarVisible(bool OffOn)
{
	if (!OffOn)
	{
		HpBar->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		HpBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void ASpaceEnemy::HpBarOpacityWork()
{

	if (currentHpBarVisualTime > HpBarVisualTime)
	{
		currentHpBarOpacity -= opacitySpeed;

		if (currentHpBarOpacity <= 0)
		{
			currentHpBarVisualTime = 0;
			isHpBarOpacityWork = false;
		}
	}
	else
	{
		currentHpBarVisualTime += GetWorld()->DeltaTimeSeconds;
	}


	HpBar->SetColorAndOpacity(FLinearColor(1, 1, 1, currentHpBarOpacity));

}

void ASpaceEnemy::SetHpBarOpacityWork()
{
	isHpBarOpacityWork = true;
	currentHpBarVisualTime = 0;
	currentHpBarOpacity = 1;
}


void ASpaceEnemy::ActivateTargetDisplay()
{
	float targetDistance = GetDistanceTo(target);

	if (targetDistance < activateTargetDisplayDistance)
	{
		targetDisplay->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		targetDisplay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ASpaceEnemy::MoveTargetDisplay()
{
	float targetDistance = GetDistanceTo(target);

	if (targetDistance < ArrowMoveDistance)
	{
		targetDisplay->MoveArrow((ArrowMoveDistance - targetDistance) / ArrowMoveSpeed);
	}
	else
	{
		targetDisplay->SetArrowInitPosition();
	}
}

void ASpaceEnemy::FireWork()
{
	currentFireRate += GetWorld()->DeltaTimeSeconds;

	if (currentMagazine > 0 && currentFireRate > fireRate)
	{
		currentFireRate = 0;

		Fire();

		if (!isInfinityMagazine)
		{
			currentMagazine -= 1;
			if (currentMagazine <= 0) isReloading = true;
		}
	}
}
void ASpaceEnemy::Reloading()
{
	currentReloadingDelay += GetWorld()->DeltaTimeSeconds;

	if (currentReloadingDelay > reloadingDelay)
	{
		currentReloadingDelay = 0;
		currentMagazine = magazine;
		isReloading = false;
	}
}

void ASpaceEnemy::DropItem()
{
	if (FMath::RandRange(0,100) < dropItemPercent)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AItem* item = GetWorld()->SpawnActor<AItem>(itemFactory, GetActorLocation(), GetActorRotation(), param);
		item->SetItemNumber(dropItemNumber);
	}	
}

void ASpaceEnemy::TakeDamage(float inputDamage)
{
	currentHp -= inputDamage;
}

void ASpaceEnemy::SetRemoveByThisPlayer(APlayerCPP* inputRemoveByThisPlayer)
{	
	removeByThisPlayer = inputRemoveByThisPlayer;
}