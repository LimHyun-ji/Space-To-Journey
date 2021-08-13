// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_ChildThrowing.h"
#include <Kismet/KismetMathLibrary.h>
#include "Player_Human.h"
#include <Components/ArrowComponent.h>

AEnemy_ChildThrowing::AEnemy_ChildThrowing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirePosition=CreateDefaultSubobject<UArrowComponent>(TEXT("FirePosition"));
	FirePosition->SetupAttachment(RootComponent);
}

void AEnemy_ChildThrowing::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy_ChildThrowing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		else if (distance >= 1500 && (!(FMath::Abs(InitLocation.X - GetActorLocation().X) <= 20 && FMath::Abs(InitLocation.Y - GetActorLocation().Y) <= 20))  )
			ReturnMove();
		else
			isWalk = false;


	}
	Dead();
}


void AEnemy_ChildThrowing::ReturnMove()
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
		SetActorLocation(currentLocation + GetActorForwardVector() * speed * 1.5 * GetWorld()->DeltaTimeSeconds);
	}

	/*
	//초기 생성 위치로 돌아가기
	FVector currentLocation = GetActorLocation();
	FVector dir = InitLocation - currentLocation;
	FVector newLocation;
	dir.Normalize();

	FRotator newRotation;
	returnGoalRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, InitLocation);

	if (returnGoalRotation.Yaw > GetActorRotation().Yaw)
		newRotation.Yaw = GetActorRotation().Yaw + 1;
	else if (returnGoalRotation.Yaw < GetActorRotation().Yaw)
		newRotation.Yaw = GetActorRotation().Yaw - 1;


	if (!(FMath::Abs(InitLocation.X - GetActorLocation().X) <= 10 && FMath::Abs(InitLocation.Y - GetActorLocation().Y) <= 10))
	{
		newLocation= currentLocation + dir * speed * GetWorld()->DeltaTimeSeconds;
		SetActorLocation(newLocation);

		if (!(GetActorRotation().Yaw == returnGoalRotation.Yaw))
		{
			SetActorRotation(newRotation);
		}
	}*/
}
void AEnemy_ChildThrowing::AttackMove()
{
	//기본 동작은 똑같고 attackMove & Attack만 다름
	FVector currentLocation = GetActorLocation();
	FVector targetLocation = target->GetActorLocation();
	FVector newLocation;

	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);
	newRotation.Pitch = 0;
	newRotation.Roll = 0;
	SetActorRotation(newRotation);

	//MoveToTarget
	FVector dir = targetLocation - currentLocation;
	dir.Normalize();
	newLocation = currentLocation + GetActorForwardVector() * speed * GetWorld()->DeltaTimeSeconds;
	SetActorLocation(newLocation);
	
}

void AEnemy_ChildThrowing::Attack()
{
	currentAttackDelay += GetWorld()->DeltaTimeSeconds;
	isAttack = true;
	if (currentAttackDelay > attackDelay)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AEnemy_ThrowingBall>(attackClass, FirePosition->GetComponentTransform(), param);
		currentAttackDelay = 0;
	}
}

