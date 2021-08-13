// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vehicle.generated.h"

UCLASS()
class SPACEWAR_API AVehicle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVehicle();
	
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UBoxComponent* collComp;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class USphereComponent* interactionAreaComp;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* GetOnPosition;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* GetOffPosition;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class USpringArmComponent* springArmComp;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UCameraComponent* cameraComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		bool isGetOnHuman = false;
	UPROPERTY()
		APlayerController* MyController;
	UPROPERTY()
		class APlayer_Human* player_Human;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;			

	UFUNCTION()
		void SetController(APlayerController* inputController);
	UFUNCTION()
		FTransform GetVehicleGetOnTransform();
	UFUNCTION()
		void SetGetOnHuman(class APlayer_Human* inputHuman);
	UFUNCTION()
		void GetOffVehicle();
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsGetOnHuman();

};
