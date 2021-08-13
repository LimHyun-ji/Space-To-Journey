// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle_Dragon.h"
#include "Player_Human.h"
#include "Components/BoxComponent.h"


AVehicle_Dragon::AVehicle_Dragon()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

void AVehicle_Dragon::BeginPlay()
{
	Super::BeginPlay();
}

void AVehicle_Dragon::Tick(float DeltaTime)
{
	if (isGetOnHuman)
	{

		collComp->SetSimulatePhysics(false);
		collComp->GetBodyInstance()->bLockXRotation=false;
		collComp->GetBodyInstance()->bLockYRotation=false;
		collComp->GetBodyInstance()->bLockZRotation = false;
		Move();
		player_Human->SetActorTransform(GetVehicleGetOnTransform());
	}
	else
	{
		collComp->SetSimulatePhysics(true);

		collComp->GetBodyInstance()->bLockXRotation = true;
		collComp->GetBodyInstance()->bLockYRotation = true;
		collComp->GetBodyInstance()->bLockZRotation = true;

	}

}

void AVehicle_Dragon::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("Vertical", this, &AVehicle_Dragon::Vertical);
	PlayerInputComponent->BindAxis("Horizontal", this, &AVehicle_Dragon::Horizontal);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AVehicle::GetOffVehicle);

}

void AVehicle_Dragon::Vertical(float value)
{
	dirVertical=GetActorForwardVector()*value;
}

void AVehicle_Dragon::Horizontal(float value)
{
	dirHorizontal=GetActorRightVector()*value;
}

void AVehicle_Dragon::Move()
{
	dir=dirHorizontal+dirVertical;
	dir.Normalize();

	FVector newLocation;
	newLocation=GetActorLocation()+ dir*speed*GetWorld()->DeltaTimeSeconds;

	SetActorLocation(newLocation,true);

	FRotator newRotation= Controller->GetControlRotation();

	SetActorRotation(newRotation);
}

