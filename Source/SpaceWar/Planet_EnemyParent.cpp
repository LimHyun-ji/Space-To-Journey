// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet_EnemyParent.h"
#include <Kismet/GameplayStatics.h>
#include "Player_Human.h"
#include <Components/CapsuleComponent.h>
#include "H_PlayerBullet.h"
#include "SkillAttack1.h"
#include <Kismet/KismetMathLibrary.h>
#include "Item.h"

// Sets default values
APlanet_EnemyParent::APlanet_EnemyParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapComp"));
	RootComponent = CapComp;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlanet_EnemyParent::BeginPlay()
{
	Super::BeginPlay();
	
	InitLocation = GetActorLocation();

	TArray <AActor*> obj;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Human::StaticClass(), obj);
	for (AActor* actor : obj)
	{
		target = Cast<APlayer_Human>(actor);
		if (IsValid(target))
			break;
	}
}

// Called every frame
void APlanet_EnemyParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlanet_EnemyParent::Attack(){}

void APlanet_EnemyParent::ReturnMove(){}

void APlanet_EnemyParent::AttackMove(){}

void APlanet_EnemyParent::RunAway()
{
	isMovingBackward = true;
	//target의 위치와 반대 방향으로 움직이기
	//어디로 움직일 지 방향을 구한다
	FVector currentLocation = GetActorLocation();
	FVector targetLocation = target->GetActorLocation();
	FVector dir = targetLocation - currentLocation;
	FVector newLocation;
	dir.Normalize();

	//target과 본인의 location 차이로 어느 정도 회전할지 구하기
	FRotator newRotation;
	runAwayGoalRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);//항상 타겟방향을 보게됨	
	runAwayGoalRotation.Pitch = 0;
	runAwayGoalRotation.Roll = 0;

	SetActorRotation(runAwayGoalRotation);

	newLocation = currentLocation + -dir * speed * 4 * GetWorld()->DeltaTimeSeconds;
	SetActorLocation(newLocation);
}

void APlanet_EnemyParent::OnOverlapBegin(AActor* OtherActor)
{
	playerBullet = Cast<AH_PlayerBullet>(OtherActor);
	skillAttack1 = Cast<ASkillAttack1>(OtherActor);
	if (OtherActor != this)
	{
		if (OtherActor == playerBullet)
		{
			isHit = true;
			playerBullet->ExploreEffect();
			enemyHP -= playerBullet->GetBulletDamage();

		}
		else if (OtherActor == skillAttack1)
		{
			isHit = true;
			enemyHP -= skillAttack1->GetAttackDamage();
		}
	}
}

void APlanet_EnemyParent::OnOverlapEnd(AActor* OtherActor)
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


void APlanet_EnemyParent::Dead()
{
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
			DropItem();
			Destroy();
		}
	}
}

void APlanet_EnemyParent::DropItem()
{
	if (FMath::RandRange(0, 100) < dropItemPercent)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AItem* item = GetWorld()->SpawnActor<AItem>(itemFactory, GetActorLocation(), GetActorRotation(), param);
		item->SetItemNumber(dropItemNumber);
	}
}

bool APlanet_EnemyParent::GetIsWalk()
{
	return isWalk;
}

bool APlanet_EnemyParent::GetIsHit()
{
	return isHit;
}

bool APlanet_EnemyParent::GetIsAttack()
{
	return isAttack;
}

bool APlanet_EnemyParent::GetIsDead()
{
	return isDead;
}

int APlanet_EnemyParent::GetAttackDamage()
{
	return attackDamage;
}

bool APlanet_EnemyParent::GetIsMovingBackward()
{
	return isMovingBackward;
}

