// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_ChildFarAway.h"
#include "Planet_EnemyParent.h"
#include "Player_Human.h"
#include <Kismet/KismetMathLibrary.h>

AEnemy_ChildFarAway::AEnemy_ChildFarAway()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemy_ChildFarAway::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy_ChildFarAway::Tick(float DeltaTime)
{
	isMovingBackward = false;
	isAttack = false;

	float distance = GetDistanceTo(target);
	if (!isDead && target != nullptr)
	{

		if (1000 < distance && distance < 1500)
		{
			AttackMove();
			Attack();
		}
		else if (distance <= 1000)
			RunAway();
		else if (distance >= 1500 && (!(FMath::Abs(InitLocation.X - GetActorLocation().X) <= 20 && FMath::Abs(InitLocation.Y - GetActorLocation().Y) <= 20)))
			ReturnMove();
		else
			isWalk = false;
	}
	Dead();
}

void AEnemy_ChildFarAway::ReturnMove()
{
	//if ((FMath::Abs(InitLocation.X - GetActorLocation().X) <= 20 && FMath::Abs(InitLocation.Y - GetActorLocation().Y) <= 20)) return;
	isWalk = true;
	//초기 생성 위치로 돌아가기
	FVector currentLocation = GetActorLocation();
	FVector dir = InitLocation - currentLocation;
	dir.Normalize();

	FRotator newRotation;
	returnGoalRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, InitLocation);
	returnGoalRotation.Pitch = 0;
	returnGoalRotation.Roll = 0;
	if (abs(GetActorRotation().Yaw - returnGoalRotation.Yaw) < 0.7)
	{
		newRotation.Yaw = GetActorRotation().Yaw;
	}
	else
	{
		newRotation.Yaw = GetActorRotation().Yaw + (-1 * (abs(GetActorRotation().Yaw - returnGoalRotation.Yaw) - 180)) / abs((abs(GetActorRotation().Yaw - returnGoalRotation.Yaw) - 180)) * (abs(returnGoalRotation.Yaw - GetActorRotation().Yaw) / (returnGoalRotation.Yaw - GetActorRotation().Yaw)) * 1;
	}

	newRotation.Pitch = 0;
	newRotation.Roll = 0;

	SetActorRotation(returnGoalRotation);

	
	
	if (newRotation.Yaw - 10 <= returnGoalRotation.Yaw && returnGoalRotation.Yaw <= newRotation.Yaw + 10)
	{
		SetActorLocation(currentLocation + GetActorForwardVector() * speed*1.5 * GetWorld()->DeltaTimeSeconds);
	}
	
	/*
	if (returnGoalRotation.Yaw > GetActorRotation().Yaw)
		newRotation.Yaw = GetActorRotation().Yaw + 1;
	else if (returnGoalRotation.Yaw < GetActorRotation().Yaw)
		newRotation.Yaw = GetActorRotation().Yaw - 1;

	if (!(FMath::Abs(InitLocation.X - GetActorLocation().X) <= 10 && FMath::Abs(InitLocation.Y - GetActorLocation().Y) <= 10))
	{
		SetActorLocation(currentLocation + dir * speed * deltaTime);
		if (!(GetActorRotation().Yaw == returnGoalRotation.Yaw))
		{
			SetActorRotation(newRotation);
		}
	}*/

	//바들바들 떨리는 현상??

}


void AEnemy_ChildFarAway::AttackMove()
{
	FVector currentLocation = GetActorLocation();
	FVector targetLocation = target->GetActorLocation();

	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);
	newRotation.Pitch = 0;
	newRotation.Roll = 0;
	SetActorRotation(newRotation);
}

void AEnemy_ChildFarAway::Attack()
{

	currentAttackDelay += GetWorld()->DeltaTimeSeconds;
	isAttack = true;
	if (currentAttackDelay>attackDelay && attackClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AEnemy_FarAwayAttack>(attackClass, target->GetActorLocation(), target->GetActorRotation(), param);
		currentAttackDelay=0;
	}
}