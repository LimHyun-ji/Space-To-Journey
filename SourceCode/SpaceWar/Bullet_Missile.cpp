// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet_Missile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Explosion.h"
#include "SpaceEnemy.h"
#include "PlayerCPP.h"
// Sets default values
ABullet_Missile::ABullet_Missile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	RootComponent = collComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(collComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ABullet_Missile::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void ABullet_Missile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();

	if (isHoming || isRealTimeHoming || isAutoTargetHoming)	Turn();
	
	if(!isInfinityLifeTime) LifeTime();
}


void ABullet_Missile::Move()
{
	dir = GetActorForwardVector();
	dir.Normalize();

	FVector P0 = GetActorLocation();
	FVector vel = speed * GetWorld()->DeltaTimeSeconds * dir;
	FVector P = vel + P0;

	SetActorLocation(P,true);
}

void ABullet_Missile::Turn()
{
	if(isIncreaseHomingAbility) turnSpeed += 0.05;

	FVector currentLocation = GetActorLocation();
	FRotator currentRotation = GetActorRotation();
	
	FRotator goalRotation;

	if (isAutoTargetHoming)
	{
		TArray <AActor*> obj;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpaceEnemy::StaticClass(), obj);
		ASpaceEnemy* Enemy = NULL;

		float distance;
		float minDistance = 50000;
		for (AActor* actor : obj)
		{
			Enemy = Cast<ASpaceEnemy>(actor);
			
			if (IsValid(Enemy))
			{
				distance = GetDistanceTo(Enemy);
				
				if (distance < 50000)
				{
					if (minDistance > distance)
					{						
						minDistance = distance;
						target = actor;														
					}					
				}
			}			
		}
		if (minDistance == 50000)
		{

			isAutoTargetHoming = false;
			return;
		}

		goalRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, target->GetActorLocation());
	}
	else if(isRealTimeHoming) goalRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, target->GetActorLocation());
	else if(isHoming) goalRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);

	FRotator newRotation;

	
	float betweenAngle_Yaw = 0;
	float betweenAngle_Pitch = 0;
	float turnSpeed_Yaw = turnSpeed;
	float turnSpeed_Pitch = turnSpeed;


	if (abs(currentRotation.Yaw - goalRotation.Yaw) < 0.05)
	{
		newRotation.Yaw = currentRotation.Yaw;
	}
	else
	{
		newRotation.Yaw = currentRotation.Yaw + (-1 * (abs(currentRotation.Yaw - goalRotation.Yaw) - 180)) / abs((abs(currentRotation.Yaw - goalRotation.Yaw) - 180)) * (abs(goalRotation.Yaw - currentRotation.Yaw) / (goalRotation.Yaw - currentRotation.Yaw)) * turnSpeed_Yaw;
	}
	//(-1 * (abs(currentRotation.Yaw - goalRotation.Yaw) - 180)) / abs((abs(currentRotation.Yaw - goalRotation.Yaw) - 180))			//두 각도 차이가 180이 넘어가면 각도가 더 작은 방향으로 회전하게함.   ex) 10도 270도 일때 더 큰 각도로 회전하는것 방지한것임
	//(abs(goalRotation.Yaw - currentRotation.Yaw)/(goalRotation.Yaw - currentRotation.Yaw))	//왼쪽으로 회전할지 오른쪽으로 회전할지

	if (abs(currentRotation.Pitch - goalRotation.Pitch) < 0.05)
	{
		newRotation.Pitch = currentRotation.Pitch;
		
	}
	else
	{
		//newRotation.Pitch = currentRotation.Pitch + (abs(goalRotation.Pitch - currentRotation.Pitch) / (goalRotation.Pitch - currentRotation.Pitch)) * turnSpeed_Pitch;
		newRotation.Pitch = currentRotation.Pitch + (-1 * (abs(currentRotation.Pitch - goalRotation.Pitch) - 180)) / abs((abs(currentRotation.Pitch - goalRotation.Pitch) - 180)) * (abs(goalRotation.Pitch - currentRotation.Pitch) / (goalRotation.Pitch - currentRotation.Pitch)) * turnSpeed_Pitch;
		
	}



	SetActorRotation(newRotation);
}

void ABullet_Missile::LifeTime()
{
	currentLifeTime += GetWorld()->DeltaTimeSeconds;

	if (currentLifeTime > lifeTime)
	{
		Dead();
	}
}

void ABullet_Missile::Dead()
{
	SpawnExplore();
	Destroy();
}

void ABullet_Missile::OnCollisionEnter_Overlab(AActor* OtherActor)
{
	ASpaceEnemy* spaceEnemy = Cast<ASpaceEnemy>(OtherActor);
	APlayerCPP* player = Cast<APlayerCPP>(OtherActor);

	if (IsValid(spaceEnemy))
	{
		Dead();
	}
	else if (IsValid(player))
	{
		Dead();
	}
}

void ABullet_Missile::OnCollisionEnter_Hit(AActor* OtherActor)
{
	Dead();
}

float ABullet_Missile::GetBulletDamage()
{
	return bulletDamage;
}

void ABullet_Missile::SpawnExplore()
{
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;	
	AExplosion* explosion = GetWorld()->SpawnActor<AExplosion>(explosionFactory, GetActorLocation(), GetActorRotation(), param);

	if (bulletOwner != NULL)
	{		
		explosion->SetBulletOwner(bulletOwner);		
	}
	
	
	
}

void ABullet_Missile::SetLifeTime(float inputLifeTime, bool setIsInfinityLifeTime = false)
{
	lifeTime = FMath::RandRange(inputLifeTime, inputLifeTime);
	isInfinityLifeTime = setIsInfinityLifeTime;
}

float ABullet_Missile::GetBulletSpeed()
{
	return speed;
}

void ABullet_Missile::SetBulletSpeed(float inputSpeed)
{
	speed = inputSpeed;
}

void ABullet_Missile::SetHomingMissile(FVector inputTargetLocation,float inputTurnValue)
{
	isHoming = true;
	targetLocation = inputTargetLocation;
	turnSpeed = inputTurnValue;
}

void ABullet_Missile::SetRealTimeHomingMissile(AActor* inputTarget, float inputTurnValue)
{
	isRealTimeHoming = true;
	
	turnSpeed = inputTurnValue;
	target = inputTarget;

}

void ABullet_Missile::SetAutoTargetHomingMissile(float inputTurnValue)
{
	isAutoTargetHoming = true;
	turnSpeed = inputTurnValue;	
}


void ABullet_Missile::SetBulletOwner(APlayerCPP* inputBulletOwner)
{
	bulletOwner = inputBulletOwner;
}

APlayerCPP* ABullet_Missile::GetBulletOwner()
{
	return bulletOwner;
}

void ABullet_Missile::SetIsIncreaseHomingAbility(bool inputBoolean)
{
	isIncreaseHomingAbility = inputBoolean;
}