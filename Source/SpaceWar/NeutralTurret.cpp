// Fill out your copyright notice in the Description page of Project Settings.


#include "NeutralTurret.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include <Components/ArrowComponent.h>
#include "Bullet_Missile.h"
#include "SpaceEnemy.h"
// Sets default values
ANeutralTurret::ANeutralTurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	RootComponent = collComp;

	fireActivateCollComp = CreateDefaultSubobject<USphereComponent>(TEXT("Fire Activation Area"));
	fireActivateCollComp->SetupAttachment(collComp);

	turretBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBody Mesh"));
	turretBodyMesh->SetupAttachment(collComp);
	turretBodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	turretGunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretGun Mesh"));
	turretGunMesh->SetupAttachment(turretBodyMesh);
	turretGunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	firePosition1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position1"));
	firePosition1->SetupAttachment(turretGunMesh);
}

// Called when the game starts or when spawned
void ANeutralTurret::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANeutralTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isFireActivate)
	{
		
		if (!isTurning)	FireWork();
		
		Turn();		
	}

	if (detectedEnemyCount == 0)
	{
		
		isFireActivate = false;
	}

}

void ANeutralTurret::Turn()
{
	FVector currentLocation = turretGunMesh->GetComponentLocation();
	FRotator currentRotation = turretGunMesh->GetComponentRotation();
	FVector targetLocation = spaceEnemy[0]->GetActorLocation();

	FRotator goalRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);
	FRotator newRotation;

	
	if (abs(currentRotation.Yaw - goalRotation.Yaw) < 0.7)
	{
		newRotation.Yaw = currentRotation.Yaw;
	}
	else
	{		
		newRotation.Yaw = currentRotation.Yaw + (-1 * (abs(currentRotation.Yaw - goalRotation.Yaw) - 180)) / abs((abs(currentRotation.Yaw - goalRotation.Yaw) - 180)) * (abs(goalRotation.Yaw - currentRotation.Yaw)/(goalRotation.Yaw - currentRotation.Yaw))*turnSpeed;
	}
	//(-1 * (abs(currentRotation.Yaw - goalRotation.Yaw) - 180)) / abs((abs(currentRotation.Yaw - goalRotation.Yaw) - 180))			//두 각도 차이가 180이 넘어가면 각도가 더 작은 방향으로 회전하게함.   ex) 10도 270도 일때 더 큰 각도로 회전하는것 방지한것임
	//(abs(goalRotation.Yaw - currentRotation.Yaw)/(goalRotation.Yaw - currentRotation.Yaw))	//왼쪽으로 회전할지 오른쪽으로 회전할지

	if (abs(currentRotation.Pitch - goalRotation.Pitch) < 0.7)
	{
		newRotation.Pitch = currentRotation.Pitch;
	}
	else
	{
		newRotation.Pitch = currentRotation.Pitch + (abs(goalRotation.Pitch - currentRotation.Pitch) / (goalRotation.Pitch - currentRotation.Pitch)) * turnSpeed;
	}


	if (abs(currentRotation.Pitch - goalRotation.Pitch) < 0.7 && abs(currentRotation.Yaw - goalRotation.Yaw) < 0.7) isTurning = false;
	else isTurning = true;

	turretGunMesh->SetWorldRotation(FRotator(newRotation.Pitch, newRotation.Yaw, 0));	

}

void ANeutralTurret::FireWork()
{
	currentFireRate += GetWorld()->DeltaTimeSeconds;

	targetDistance = GetDistanceTo(spaceEnemy[0]);
	
	

	if (currentFireRate > fireRate)
	{
		currentFireRate = 0;
		Fire();
	}
}

void ANeutralTurret::Fire()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), fireEffectFactory, firePosition1->GetComponentLocation(), firePosition1->GetComponentRotation(), GetActorScale3D());

	float spread_Pitch = FMath::RandRange(-spread, spread);
	float spread_Yaw = FMath::RandRange(-spread, spread);

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABullet_Missile* bullet = GetWorld()->SpawnActor<ABullet_Missile>(BulletFactory, firePosition1->GetComponentLocation(), firePosition1->GetComponentRotation() + FRotator(spread_Pitch, spread_Yaw, 0),param);
	bulletSpeed = bullet->GetBulletSpeed();
	bullet->SetLifeTime((targetDistance / bulletSpeed) + FMath::RandRange(-0.2f,0.2f));
	bullet->SetHomingMissile(spaceEnemy[0]->GetActorLocation(),1);
}



void ANeutralTurret::OnCollision_Overlap(AActor* OtherActor)
{
	ASpaceEnemy* spaceEnemyTemp = Cast<ASpaceEnemy>(OtherActor);
	//AEnemy_Hunter* enemy_Hunter = Cast<AEnemy_Hunter>(OtherActor);

	if (IsValid(spaceEnemyTemp))
	{
		detectedEnemyCount++;
		spaceEnemy.Add(spaceEnemyTemp);
		isFireActivate = true;		
	}
	
}

void ANeutralTurret::OnCollision_EndOverlap(AActor* OtherActor)
{
	ASpaceEnemy* spaceEnemyTemp = Cast<ASpaceEnemy>(OtherActor);
	//AEnemy_Hunter* enemy_Hunter = Cast<AEnemy_Hunter>(OtherActor);
	

	if (IsValid(spaceEnemyTemp))
	{
		detectedEnemyCount--;
		//enemy_FreeFly.RemoveAt(0);
		spaceEnemy.Remove(spaceEnemyTemp);
	}

	if (detectedEnemyCount == 0)
	{
		isFireActivate = false;
	}
}
