// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

UCLASS()
class SPACEWAR_API AExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosion();

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class USphereComponent* collComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		float GetDamage();

	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnCollision_Overlap(AActor* OtherActor);

	
	UFUNCTION()
		void DestroyWork();
	UFUNCTION()
		void DisableCollisionWork();

	UFUNCTION()
		void SetBulletOwner(APlayerCPP* inputBulletOwner);
	UFUNCTION()
		class APlayerCPP* GetBulletOwner();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float damage = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess))
		class USoundBase* explosionSound;
	UPROPERTY(EditDefaultsOnly, Category = "Bgm")
		class APlayerCPP* bulletOwner = NULL;


	UPROPERTY()
		float destroyTime = 1;
	UPROPERTY()
		float currentDestroyTime = 0;
	UPROPERTY()
		float collisionTime = 0.1;
	UPROPERTY()
		float currnetCollisionTime = 0;

	UPROPERTY()
		uint8 count = 0;
};
