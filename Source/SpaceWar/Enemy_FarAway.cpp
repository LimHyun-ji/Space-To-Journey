// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_FarAway.h"
#include <Kismet/GameplayStatics.h>
#include "SpawnManager.h"
#include "Player_Human.h"
#include <Kismet/KismetMathLibrary.h>
#include "Enemy_FarAwayAttack.h"
#include "H_PlayerBullet.h"
#include "SkillAttack1.h"
#include <Components/CapsuleComponent.h>

// Sets default values
AEnemy_FarAway::AEnemy_FarAway()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapComp"));
	RootComponent = CapComp;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemy_FarAway::BeginPlay()
{
	Super::BeginPlay();
	InitLocation = GetActorLocation();

	TArray <AActor*> obj1;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Human::StaticClass(), obj1);
	for (AActor* actor : obj1)
	{
		target = Cast<APlayer_Human>(actor);
		if (IsValid(target))
			break;
	}

}

// Called every frame
void AEnemy_FarAway::Tick(float DeltaTime)
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
			RunAway(DeltaTime);
		else if (distance >= 1500)
			ReturnMove(DeltaTime);

	}

	/////
	if (enemyHP < 0)
	{
		deathCountDown -= GetWorld()->DeltaTimeSeconds;

		if (isDead)
		{
			CapComp->SetSimulatePhysics(false);
			CapComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
						
		isDead = true;
		if (deathCountDown < 0)
		{
			target->SetPlayerMP(target->GetPlayerMP() + 10);
			Destroy();
		}
	}		

}

void AEnemy_FarAway::RunAway(float deltaTime)
{

	isMovingBackward = true;
	//target의 위치와 반대 방향으로 움직이기
	//어디로 움직일 지 방향을 구한다
	FVector currentLocation = GetActorLocation();
	FVector targetLocation = target->GetActorLocation();
	FVector dir = targetLocation - currentLocation;
	dir.Normalize();
	//target과 본인의 location 차이로 어느 정도 회전할지 구하기
	FRotator newRotation;
	runAwayGoalRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);//항상 타겟방향을 보게됨	
	runAwayGoalRotation.Pitch = 0;
	runAwayGoalRotation.Roll = 0;

	SetActorRotation(runAwayGoalRotation);
	SetActorLocation(currentLocation + -dir * speed * 4 * deltaTime);

}
void AEnemy_FarAway::ReturnMove(float deltaTime)
{
	//초기 생성 위치로 돌아가기
	FVector currentLocation = GetActorLocation();
	FVector dir = InitLocation - currentLocation;
	dir.Normalize();

	FRotator newRotation;
	returnGoalRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, InitLocation);

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

	SetActorRotation(newRotation);

	if (!(FMath::Abs(InitLocation.X - GetActorLocation().X) <= 10 && FMath::Abs(InitLocation.Y - GetActorLocation().Y) <= 10))
	{
		if (newRotation.Yaw - 10 <= returnGoalRotation.Yaw && returnGoalRotation.Yaw <= newRotation.Yaw + 10)
		{
			SetActorLocation(currentLocation + dir * speed * deltaTime);
		}
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

void AEnemy_FarAway::AttackMove()
{
	FVector currentLocation = GetActorLocation();
	FVector targetLocation = target->GetActorLocation();

	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);
	newRotation.Pitch = 0;
	newRotation.Roll = 0;

	SetActorRotation(newRotation);
}

void AEnemy_FarAway::Attack()
{

	spawnCountDown -= GetWorld()->DeltaTimeSeconds;
	isAttack = true;
	if (spawnCountDown < 0)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AEnemy_FarAwayAttack>(attack, target->GetActorLocation(), target->GetActorRotation(), param);
		spawnCountDown = 1.0f;
	}
}

//총에 맞았는지 판별
void AEnemy_FarAway::OnOverlapBegin(AActor* OtherActor)
{
	playerBullet = Cast<AH_PlayerBullet>(OtherActor);
	skillAttack1 = Cast<ASkillAttack1>(OtherActor);

	if (OtherActor != this)
	{
		if (OtherActor == playerBullet)
		{
			isHit = true;
			enemyHP -= playerBullet->GetBulletDamage();
			playerBullet->ExploreEffect();

		}
		else if (OtherActor == skillAttack1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Skill Hit"));
			isHit = true;
			enemyHP -= skillAttack1->GetAttackDamage();
		}
	}

}
void AEnemy_FarAway::OnOverlapEnd(AActor* OtherActor)
{
	playerBullet = Cast<AH_PlayerBullet>(OtherActor);
	skillAttack1 = Cast<ASkillAttack1>(OtherActor);

	if (OtherActor != this)
	{
		if (OtherActor == playerBullet || skillAttack1)
		{
			isHit = false;
		}
	}

}



bool AEnemy_FarAway::GetIsHit()
{
	return isHit;
}

bool AEnemy_FarAway::GetIsAttack()
{
	return isAttack;
}

bool AEnemy_FarAway::GetIsDead()
{
	return isDead;
}

bool AEnemy_FarAway::GetIsMovingBackward()
{
	return isMovingBackward;
}

