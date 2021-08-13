// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vehicle.h"
#include "HarborBoard.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API AHarborBoard : public AVehicle
{
	GENERATED_BODY()

public:
	AHarborBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void MoveForward(float value);
	UFUNCTION()
		void InputHorizontal(float value);
	UFUNCTION()
		void Move();
	UFUNCTION()
		void RotationCamera();
	UFUNCTION()
		void PitchCamera(float value);
	UFUNCTION()
		void YawCamera(float value);

private:
	UPROPERTY()
		FVector dir_For;
	UPROPERTY()
		FVector dir_Hor;
	UPROPERTY()
		FVector dir;
	UPROPERTY()
		FVector CameraInput;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float speed = 1000;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float maxCameraPitchAngle;
};
