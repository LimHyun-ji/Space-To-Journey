// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBullet.generated.h"

UCLASS()
class SPACEWAR_API APlayerBullet : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Component")
	class UBoxComponent* collComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	class UStaticMeshComponent* meshComp;

public:	
	// Sets default values for this actor's properties
	APlayerBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		class UParticleSystem* exploreFactory;
	UPROPERTY(EditDefaultsOnly, Category = "Bgm")
		class USoundBase* fireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Bgm")
		class APlayerCPP* bulletOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	float speed = 1000;
	UPROPERTY()
	FVector dir = FVector(0, 0, 0);

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	float lifeTime = 4;
	UPROPERTY()
	float currentLifeTime = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	float bulletDamage = 5;

	UFUNCTION()
		void Move();
	UFUNCTION()
		void LifeTime();

	UFUNCTION(BlueprintCallable, Category = "Code")
	void OnCollisionEnter_Overlab(AActor* OtherActor);

	UFUNCTION()
		float GetBulletDamage();
	UFUNCTION()
		void ExploreEffect();
	UFUNCTION()
		void SetBulletDamage(float inputDamage);
	UFUNCTION()
		void SetBulletOwner(APlayerCPP* inputBulletOwner);
	UFUNCTION()
		class APlayerCPP* GetBulletOwner();
};
