// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AllBulletParent.h"
#include "Bullet_Missile.generated.h"

UCLASS()
class SPACEWAR_API ABullet_Missile : public AAllBulletParent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet_Missile();

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UBoxComponent* collComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* meshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		TSubclassOf <class AExplosion> explosionFactory;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float speed = 1000;
	UPROPERTY()
		FVector dir = FVector(0, 0, 0);
	UPROPERTY()
		FVector targetLocation = FVector(0, 0, 0);

	UPROPERTY()
		float lifeTime = 5;
	UPROPERTY()
		bool isInfinityLifeTime = false;
	UPROPERTY()
		float currentLifeTime = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float bulletDamage = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")	//모두 사용가능. 지정된 타겟의 초기 위치를 찾아감
		bool isHoming = false;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")	//모두 사용가능. 지정된 타겟을 실시간으로 찾아감
		bool isRealTimeHoming = false;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")	//플래이어만 사용 가능. 자동으로 가장 가까운 적을 찾아감
		bool isAutoTargetHoming = false;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float turnSpeed = 0;

	UPROPERTY()
		AActor* target;
	UPROPERTY(EditDefaultsOnly, Category = "Bgm")
		class APlayerCPP* bulletOwner = NULL;

	UPROPERTY()
		bool isIncreaseHomingAbility = true;

	UFUNCTION()
		void Move();
	UFUNCTION()
		void Turn();
	UFUNCTION()
		void LifeTime();
	UFUNCTION()
		void Dead();
	UFUNCTION()
		void SetBulletSpeed(float inputSpeed);
	UFUNCTION()
		void SetLifeTime(float inputLifeTime, bool setIsInfinityLifeTime);

	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnCollisionEnter_Overlab(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnCollisionEnter_Hit(AActor* OtherActor);

	UFUNCTION()
		float GetBulletDamage();
	UFUNCTION()
		void SpawnExplore();
	UFUNCTION()
		float GetBulletSpeed();

	UFUNCTION()
		void SetHomingMissile(FVector intputTargetLocation, float inputTurnValue);	//초기 위치를 추적
	UFUNCTION()
		void SetRealTimeHomingMissile(AActor* inputTarget, float inputTurnValue);	//주어진 대상을 실시간으로 추적
	UFUNCTION()
		void SetAutoTargetHomingMissile(float inputTurnValue);	//대상을 스스로 찾아 추적

	UFUNCTION()
		void SetBulletOwner(APlayerCPP* inputBulletOwner);
	UFUNCTION()
		class APlayerCPP* GetBulletOwner();

	UFUNCTION()
		void SetIsIncreaseHomingAbility(bool inputBoolean);
};
