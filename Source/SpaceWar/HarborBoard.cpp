// Fill out your copyright notice in the Description page of Project Settings.


#include "HarborBoard.h"
#include "Player_Human.h"

AHarborBoard::AHarborBoard()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHarborBoard::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHarborBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isGetOnHuman)
	{
		Move();
		RotationCamera();
		player_Human->SetActorTransform(GetVehicleGetOnTransform());
	}
	
	

}

// Called to bind functionality to input
void AHarborBoard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &AHarborBoard::MoveForward);
	PlayerInputComponent->BindAxis("Horizontal", this, &AHarborBoard::InputHorizontal);
	PlayerInputComponent->BindAxis("CameraPitch", this, &AHarborBoard::PitchCamera);
	PlayerInputComponent->BindAxis("CameraYaw", this, &AHarborBoard::YawCamera);

	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AVehicle::GetOffVehicle);

}

void AHarborBoard::Move()
{
	dir = dir_For + dir_Hor;
	dir.Normalize();

	FVector vel = speed * dir * GetWorld()->DeltaTimeSeconds;
	FVector P0 = GetActorLocation();
	FVector P = vel + P0;

	SetActorLocation(P, true);
}

void AHarborBoard::MoveForward(float value)
{	
	dir_For = GetActorForwardVector()* value;
}

void AHarborBoard::InputHorizontal(float value)
{
	dir_Hor = GetActorRightVector()*value;
}

void AHarborBoard::PitchCamera(float value)
{
	CameraInput.Y = value;
}


void AHarborBoard::YawCamera(float value)
{
	CameraInput.X = value;
}

void AHarborBoard::RotationCamera()
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += CameraInput.X;
	NewRotation.Pitch += CameraInput.Y;

	if (abs(NewRotation.Pitch) > maxCameraPitchAngle) NewRotation.Pitch = (abs(NewRotation.Pitch) / NewRotation.Pitch) * maxCameraPitchAngle;
	//NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, 80.0f);

	/*
	//Rotate our camera's pitch, but limit it so we're always looking downward
	{
		FRotator NewRotation = springArmComp->GetComponentRotation();
		NewRotation.Pitch = NewRotation.Pitch + CameraInput.Y;
		NewRotation.Roll = 0;
		springArmComp->SetWorldRotation(NewRotation);
	}*/

	NewRotation.Roll += 0;

	SetActorRotation(NewRotation);
}