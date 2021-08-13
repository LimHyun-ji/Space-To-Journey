// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vehicle.h"
#include "Vehicle_Dragon.generated.h"

/**
 *
 */
UCLASS()
class SPACEWAR_API AVehicle_Dragon : public AVehicle
{
	GENERATED_BODY()

public:
	AVehicle_Dragon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Called every frame
	virtual void Tick(float DeltaTime) override;

	//Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void Vertical(float value);
	UFUNCTION()
		void Horizontal(float value);
	UFUNCTION()
		void Move();
	// 	UFUNCTION()
	// 		void RotationCamera();
	// 	UFUNCTION()
	// 		void PitchCamera(float value);
	// 	UFUNCTION()
	// 		void YawCamera(float value);

private:
// 	UPROPERTY()
// 		FVector dir_Forward;
// 	UPROPERTY()
// 		FVector dir_Horizontal;
// 	UPROPERTY()
// 		FVector dir;
// 	UPROPERTY()
// 		FVector CameraInput;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float speed = 1000;

	UPROPERTY()
	FVector dirVertical;
	UPROPERTY()
	FVector dirHorizontal;
	UPROPERTY()
	FVector dir;
};
