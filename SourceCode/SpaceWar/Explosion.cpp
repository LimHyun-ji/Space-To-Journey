// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceEnemy.h"
#include "PlayerCPP.h"

// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Box"));	
	RootComponent = collComp;
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), explosionSound, GetActorLocation());

	collComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void AExplosion::Tick(float DeltaTime)
{	

	Super::Tick(DeltaTime);
	DestroyWork();
	DisableCollisionWork();
	
	
	if(count >= 2) collComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	else if(count >= 1) collComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	count++;

}


void AExplosion::DestroyWork()
{
	currentDestroyTime += GetWorld()->DeltaTimeSeconds;

	if (currentDestroyTime > destroyTime)
	{
		Destroy();
	}
}

void AExplosion::DisableCollisionWork()
{
	currnetCollisionTime += GetWorld()->DeltaTimeSeconds;
	if (currnetCollisionTime > collisionTime)
	{
		collComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

float AExplosion::GetDamage()
{
	return damage;
}

void AExplosion::OnCollision_Overlap(AActor* OtherActor)
{
	ASpaceEnemy* spaceEnemy = Cast<ASpaceEnemy>(OtherActor);
	APlayerCPP* player = Cast<APlayerCPP>(OtherActor);

	if (IsValid(spaceEnemy))
	{
		spaceEnemy->TakeDamage(damage);
		
		if (bulletOwner != NULL)
		{
			UE_LOG(LogTemp, Warning, TEXT("explosion : %s"), *bulletOwner->GetName());
			spaceEnemy->SetRemoveByThisPlayer(bulletOwner);
		}		
	}
	else if (IsValid(player))
	{
		player->TakeDamage(damage);
	}
}

void AExplosion::SetBulletOwner(APlayerCPP* inputBulletOwner)
{
	bulletOwner = inputBulletOwner;
}

APlayerCPP* AExplosion::GetBulletOwner()
{
	return bulletOwner;
}