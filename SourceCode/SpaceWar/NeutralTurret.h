// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeutralTurret.generated.h"

UCLASS()
class SPACEWAR_API ANeutralTurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANeutralTurret();

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UBoxComponent* collComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class USphereComponent* fireActivateCollComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* turretBodyMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* turretGunMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* firePosition1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnCollision_Overlap(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnCollision_EndOverlap(AActor* OtherActor);
	UFUNCTION()
		void Turn();
	UFUNCTION()
		void FireWork();
	UFUNCTION()
		void Fire();

private:

	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess))
		TSubclassOf <class ABullet_Missile> BulletFactory;
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess))
		class UParticleSystem* fireEffectFactory;
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess))
		class USoundBase* fireSound;
	UPROPERTY()
		TArray <class ASpaceEnemy*> spaceEnemy;
	UPROPERTY()
		uint8 detectedEnemyCount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float fireRate = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float spread = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float turnSpeed = 2;
	UPROPERTY()
		float currentFireRate = 0;
	UPROPERTY()
		bool isFireActivate = false;
	UPROPERTY()
		float targetDistance;
	UPROPERTY()
		float bulletSpeed;
	UPROPERTY()
		bool isTurning = true;
};
