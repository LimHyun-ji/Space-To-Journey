// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBullet_Parent.h"
#include "Enemy_BulletBall.generated.h"

UCLASS()
class SPACEWAR_API AEnemy_BulletBall : public AEnemyBullet_Parent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_BulletBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		FVector dir = FVector(0, 0, 0);

	UFUNCTION()
		void Move();
	

	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnCollisionEnter_Overlab(AActor* OtherActor);
	

};
