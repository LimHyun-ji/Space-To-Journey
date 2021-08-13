// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletBox.generated.h"

UCLASS()
class SPACEWAR_API ABulletBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABulletBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* MeshComp;

	UPROPERTY()
		class APlayer_Human* target;

	UFUNCTION(BluePrintCallable, Category = "Code")
		void OnCollision(AActor* OtherActor);
};
