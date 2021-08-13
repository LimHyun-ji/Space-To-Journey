// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DragonInteractionArea.generated.h"

UCLASS()
class SPACEWAR_API ADragonInteractionArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADragonInteractionArea();

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class APlayer_Human* target;
	UPROPERTY()
	class AEnemy_Dragon* dragon;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	class UParticleSystem* openKeyEffect;
	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* EffectPosition;
	UPROPERTY()
	class UParticleSystemComponent* fireParticle;
	
	UPROPERTY()
	bool wakeUpDragon=false;
	
	UPROPERTY()
	bool canInteract=true;
	UPROPERTY()
	bool isSpawnEmitter=false;
	UPROPERTY(EditAnywhere, Category="Settings")
	int orderNum;

	UFUNCTION(BlueprintCallable, Category = "Code")
		void BeginOverlapInteraction(AActor* OtherActor);

	UFUNCTION(BluePrintCallable, Category = "Code")
		void EndOverlapInteraction(AActor* OtherActor);
};
