// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TestPawn.generated.h"

UCLASS()
class SPACEWAR_API ATestPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATestPawn();

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UBoxComponent* collComp;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class USpringArmComponent* springArmComp;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UCameraComponent* cameraComp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY()
	class ASpaceWarGameModeBase* gameMode;
	UPROPERTY()
		class APlayerController* MyController;
	

	void ReloadPress();
	void InputHorizontal(float value);
	void takeController();
	void Interaction();
};
