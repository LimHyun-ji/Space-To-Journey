// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Throwing.h"
#include "Player_Human.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include "Enemy_ThrowingBall.h"
#include <Components/ArrowComponent.h>
#include "H_PlayerBullet.h"
#include "SkillAttack1.h"
#include <Components/CapsuleComponent.h>

// Sets default values
AEnemy_Throwing::AEnemy_Throwing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapComp"));
	RootComponent = CapComp;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComponent);

	FirePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePosition"));
	FirePosition->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy_Throwing::BeginPlay()
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
void AEnemy_Throwing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	isMovingBackward = false;
	isAttack = false;
	float distance = GetDistanceTo(target);
	if (!isDead && target != nullptr)
	{
		if (1000 < distance && distance < 1500)
		{
			AttackMove(DeltaTime);
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

void AEnemy_Throwing::RunAway(float deltaTime)
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
	//SetActorRotation(newRotation);	//newRotation의 값을 어디서 설정하지를 않았는데 플래이어를 봄. 어떻게 한거지	
	SetActorRotation(runAwayGoalRotation);
	SetActorLocation(currentLocation + -dir * speed * 4 * deltaTime);

}
void AEnemy_Throwing::ReturnMove(float deltaTime)
{

	//초기 생성 위치로 돌아가기
	FVector currentLocation = GetActorLocation();
	FVector dir = InitLocation - currentLocation;
	dir.Normalize();

	FRotator newRotation;
	returnGoalRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, InitLocation);

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
	}
}

void AEnemy_Throwing::AttackMove(float deltaTime)
{
	//기본 동작은 똑같고 attackMove & Attack만 다름
	FVector currentLocation = GetActorLocation();
	FVector targetLocation = target->GetActorLocation();

	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);
	SetActorRotation(newRotation);

	//MoveToTarget
	FVector dir = targetLocation - currentLocation;
	dir.Normalize();
	SetActorLocation(currentLocation + dir * speed * deltaTime);
}

void AEnemy_Throwing::Attack()
{
	spawnCountDown -= GetWorld()->DeltaTimeSeconds;
	isAttack = true;
	if (spawnCountDown < 0)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AEnemy_ThrowingBall>(attack, FirePosition->GetComponentTransform(), param);
		spawnCountDown = 1.0f;
	}

}

//총에 맞았는지 판별
void AEnemy_Throwing::OnOverlapBegin(AActor* OtherActor)
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
			UE_LOG(LogTemp, Warning, TEXT("Skill Hit"));
			isHit = true;
			enemyHP -= skillAttack1->GetAttackDamage();
		}
	}

}
void AEnemy_Throwing::OnOverlapEnd(AActor* OtherActor)
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



bool AEnemy_Throwing::GetIsHit()
{
	return isHit;
}

bool AEnemy_Throwing::GetIsAttack()
{
	return isAttack;
}

bool AEnemy_Throwing::GetIsDead()
{
	return isDead;
}

bool AEnemy_Throwing::GetIsMovingBackward()
{
	return isMovingBackward;
}


