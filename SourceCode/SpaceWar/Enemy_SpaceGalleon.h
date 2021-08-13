// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceEnemy.h"
#include "Enemy_SpaceGalleon.generated.h"

UCLASS()
class SPACEWAR_API AEnemy_SpaceGalleon : public ASpaceEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy_SpaceGalleon(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* firePosition1;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* firePosition2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;





	UPROPERTY(EditDefaultsOnly, Category = "stats")
		float speed = 300;
	UPROPERTY()
		FVector dir = FVector(0, 0, 0);
	UPROPERTY(EditDefaultsOnly, Category = "stats")
		float fireActivationDistance = 12000;
	UPROPERTY()
		bool isFireActivation = false;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		bool isRealTimeHoming = false;
	////////	FUNCTION

	UFUNCTION()
		void Move();
	UFUNCTION()
		void Dead();
	UFUNCTION()
		void Fire() override;
	UFUNCTION()
		void FirePositionLookTarget();
	UFUNCTION()
		void FireActivate();


	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnCollisionEnter_Overlab(AActor* OtherActor);


};
