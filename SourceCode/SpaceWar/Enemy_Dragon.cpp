// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Dragon.h"
#include <Components/ArrowComponent.h>
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Player_Human.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/CapsuleComponent.h>
#include <Components/BoxComponent.h>
#include "SkillAttack1.h"
#include "H_PlayerBullet.h"

//드래곤 구역으로 출입시 카메라 멀어지게 해야함
// Sets default values
AEnemy_Dragon::AEnemy_Dragon()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PhysicsComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PhysicsComp"));
	RootComponent = PhysicsComp;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	FirePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePosition"));
	FirePosition->SetupAttachment(RootComponent);


	BitePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("BitePosition"));
	BitePosition->SetupAttachment(RootComponent);

	ClawPosition1 = CreateDefaultSubobject<UArrowComponent>(TEXT("ClawPosition1"));
	ClawPosition1->SetupAttachment(RootComponent);

	ClawPosition2 = CreateDefaultSubobject<UArrowComponent>(TEXT("ClawPosition2"));
	ClawPosition2->SetupAttachment(RootComponent);

	AttackInteraction = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackInteraction"));
	AttackInteraction->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy_Dragon::BeginPlay()
{
	Super::BeginPlay();
	InitLocation = GetActorLocation();
	InitRotation = GetActorRotation();

	TArray <AActor*> obj;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Human::StaticClass(), obj);
	for (AActor* actor : obj)
	{
		target = Cast<APlayer_Human>(actor);
		if (IsValid(target))
			break;
	}

}

// Called every frame
void AEnemy_Dragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!target->isDead)
	{
		if (isInteracting)
		{


			//player의 카메라 위치를 바꾸기
			playerCameraLocation = FVector(-738.030518, 0.000000, -87.71233);
			playerCameraRotation = FRotator(24.999979, 0.000000, 0.000000);

			target->FollowCamera->SetRelativeLocation(playerCameraLocation);
			target->FollowCamera->SetRelativeRotation(playerCameraRotation);


			if (!isDead)
			{

				if (currentCineCameraDelay > cineCameraDelay)
					currentPatternTime += GetWorld()->DeltaTimeSeconds;

				if (currentPatternTime > patternTime)
				{
					patternMode++;
					ResetSettings();
				}

				switch (patternMode % 5)
				{
				case 0:
				case 1:
					WakeUp();
					if (isAttackActivate)
					{

						switch (attackMode)
						{
						case 0: SpitFireBall();
							break;
						case 1: SpreadFire();
							break;
						}
					}
					break;


				case 2:
					Gliding();
					break;
				case 3:
				case 4:
				case 5:
				case 6:
					Grounding();
					break;
				}

				if (isBiteAttack && isWalking)
				{
					currentBiteSpawnTime += GetWorld()->DeltaTimeSeconds;
					if (currentBiteSpawnTime > biteSpawnTime)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BiteEffect, BitePosition->GetComponentLocation(), BitePosition->GetComponentRotation(), GetActorScale3D() - FVector(0.3, 0.3, 0.3));
						target->playerHP -= biteAttackDamage;
						currentBiteSpawnTime = 0;
					}
				}
			}

			if (dragonHP < 0)
			{



				FVector dir = GetActorUpVector();
				dir.Normalize();
				FVector newLocation;
				FRotator newRotation = GetActorRotation();
				newRotation.Pitch = 0;
				SetActorRotation(newRotation);
				newLocation = GetActorLocation() + -dir * speed / 1.5 * GetWorld()->DeltaTimeSeconds;
				if (GetActorLocation().Z > InitLocation.Z)
					SetActorLocation(newLocation);
				//else simulate physics
				isDead = true;
				target->isCatchDragon = true;
				currentDeathDelayTime += GetWorld()->DeltaTimeSeconds;

				if (currentDeathDelayTime > deathDelayTime)
				{
					playerCameraLocation = FVector(-101.318939, 0.000000, 122.368973);
					playerCameraRotation = FRotator(-10.000021, 0.000000, 0.000000);
					target->FollowCamera->SetRelativeLocation(playerCameraLocation);
					target->FollowCamera->SetRelativeRotation(playerCameraRotation);

					Destroy();
				}

			}
		}
		else
		{
			playerCameraLocation = FVector(-101.318939, 0.000000, 122.368973);
			playerCameraRotation = FRotator(-10.000021, 0.000000, 0.000000);

			target->FollowCamera->SetRelativeLocation(playerCameraLocation);
			target->FollowCamera->SetRelativeRotation(playerCameraRotation);

			attackMode = FMath::RandRange(0, 1);//0:Spread Fire    1: Spit Fire
			ReturnToInit();
			patternMode = 0;
		}
	}

}


void AEnemy_Dragon::WakeUp()
{
	//날아오르기
	//target을 바라보기
	isFlying = true;
	//PhysicsComp->SetSimulatePhysics(false);
// 	PhysicsComp->GetBodyInstance()->bLockXRotation = false;
// 	PhysicsComp->GetBodyInstance()->bLockYRotation = false;
	FVector currentLocation = GetActorLocation();
	FVector newLocation;
	FVector dir;
	FVector targetLocation = target->GetActorLocation();
	FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);


	dir = GetActorUpVector();
	if (isWakeUp)
	{
		FRotator newRotation = GetActorRotation();
		//player가 있는 방향으로 부드럽게 회전
		if (FMath::Abs(GetActorRotation().Yaw - lookRotation.Yaw) > 10)
		{
			PhysicsComp->SetSimulatePhysics(false);
			PhysicsComp->GetBodyInstance()->bLockXRotation = false;
			PhysicsComp->GetBodyInstance()->bLockYRotation = false;
			if (GetActorRotation().Yaw > lookRotation.Yaw)
				newRotation.Yaw -= 1;
			else if (GetActorRotation().Yaw <= lookRotation.Yaw)
				newRotation.Yaw += 1;

			SetActorRotation(newRotation);
		}
		else
			isWakeUp = false;
	}
	else
	{
		currentCineCameraDelay += GetWorld()->DeltaTimeSeconds;
		currentWakeUpDelay += GetWorld()->DeltaTimeSeconds;
		if (currentCineCameraDelay > cineCameraDelay)
			target->PlayerWidgetRemoveAdd(1);
		else
			target->PlayerWidgetRemoveAdd(0);
		if (currentWakeUpDelay > wakeUpDelay && currentCineCameraDelay > cineCameraDelay && InitLocation.Z + 1000 > currentLocation.Z)
		{
			//공중에 올라가기
			newLocation = currentLocation + dir * GetWorld()->DeltaTimeSeconds * speed;
			SetActorLocation(newLocation);
		}

		if (FMath::Abs(currentLocation.Z - InitLocation.Z) > 900)
		{
			//if(FMath::Abs(GetActorLocation().X - target->GetTargetLocation().X)>50)//임시방편
			SetActorRotation(lookRotation);
			isAttackActivate = true;
			currentAttackTime += GetWorld()->DeltaTimeSeconds;
			if (currentAttackTime > attackTime)
			{
				attackMode = FMath::RandRange(0, 1);//0:Spread Fire    1: Spit Fire
				currentAttackTime = 0;
			}
		}


	}

}

void AEnemy_Dragon::SpitFireBall()
{
	//공중공격1
	isSpitting = true;

	if (SpitClass)
	{
		FActorSpawnParameters params;
		params.Owner = this;
		params.Instigator = GetInstigator();
		currentSpitDelayTime += GetWorld()->DeltaTimeSeconds;
		if (currentSpitDelayTime > spitDelayTime)
		{
			ADragonFire* FireBall = GetWorld()->SpawnActor<ADragonFire>(SpitClass, FirePosition->GetComponentTransform(), params);
			currentSpitDelayTime = 0.0f;
		}

	}
}
void AEnemy_Dragon::SpreadFire()
{
	//공중공격2
	isSpreading = true;

	if (SpreadClass)
	{
		FActorSpawnParameters params;
		params.Owner = this;
		params.Instigator = GetInstigator();
		ADragonFire* FireBall = GetWorld()->SpawnActor<ADragonFire>(SpreadClass, FirePosition->GetComponentTransform(), params);
	}
}

void AEnemy_Dragon::ReturnToInit()
{
	ResetSettings();
	FVector currentLocation = GetActorLocation();
	FRotator currentRotation = GetActorRotation();
	FVector dir = InitLocation - GetActorLocation();//캐릭터가 가야하는 방향
	FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(InitLocation, currentLocation);
	//부드럽게 회전하며 움직이기

	if (FMath::Abs(GetActorRotation().Pitch - InitRotation.Pitch) >= 1)
	{
		if (InitRotation.Pitch > currentRotation.Pitch)
			currentRotation.Pitch += 1;
		else
			currentRotation.Pitch -= 1;

		SetActorRotation(currentRotation);
	}

	if (FMath::Abs(GetActorRotation().Yaw - InitRotation.Yaw) >= 1)
	{
		if (flyingPattern == 0)
			currentRotation.Yaw += 1;
		else
			currentRotation.Yaw -= 1;

		SetActorRotation(currentRotation);
	}

	dir.Normalize();
	FVector newLocation;

	if (InitLocation.Z < GetActorLocation().Z)
	{
		newLocation = GetActorLocation() + dir * GetWorld()->DeltaTimeSeconds * speed;
		SetActorLocation(newLocation);
	}
	else if (FMath::Abs(InitLocation.X - GetActorLocation().X) > 50 || FMath::Abs(InitLocation.Y - GetActorLocation().Y) > 50)
	{
		newLocation = GetActorLocation() + dir * GetWorld()->DeltaTimeSeconds * speed / 5;
		SetActorLocation(newLocation);
		isWalking = true;
	}
	else//착륙
	{
		// 		PhysicsComp->SetSimulatePhysics(true);
		PhysicsComp->GetBodyInstance()->bLockXRotation = true;
		PhysicsComp->GetBodyInstance()->bLockYRotation = true;
		isFlying = false;
	}
}



void AEnemy_Dragon::Gliding()
{

	//make gliding pattern
	isGliding = true;

	FRotator newRotation;
	FVector dir = GetActorForwardVector();
	FVector newLocation;
	targetPrevLocation = target->GetActorLocation();

	//pitch를 원위치 0으로 돌리기

	if (GetActorRotation().Pitch < 0 && !isResetPitch)
	{
		newRotation.Pitch += 1;
		SetActorRotation(newRotation);
	}
	else
		isResetPitch = true;

	if (isResetPitch)
	{
		//		UE_LOG(LogTemp, Warning, TEXT("GlidIngStartROtation yaw %f"), glidingStartRotation.Yaw);//0 인데 player가 0의 위치로 들어오거나 드래곤이 0로테이션값을 우연히 가지는 경우 Gliding안하는 문제..
		switch (flyingPattern)
		{
		case 0:
			newRotation.Yaw = GetActorRotation().Yaw + 1;
			if (FMath::Abs(glidingStartRotation.Yaw - newRotation.Yaw) > 1)
			{
				if (newRotation.Yaw > 180)
					newRotation.Yaw -= 360;
				SetActorRotation(newRotation);
				newLocation = GetActorLocation() + dir * speed * GetWorld()->DeltaTimeSeconds;
				SetActorLocation(newLocation);
			}
			else
			{
				flyingPattern = 1;
			}
			break;
		case 1:
			newRotation.Yaw = GetActorRotation().Yaw - 1;
			if (FMath::Abs(glidingStartRotation.Yaw - newRotation.Yaw) > 1)
			{
				if (newRotation.Yaw < -180)
					newRotation.Yaw += 360;
				SetActorRotation(newRotation);
				newLocation = GetActorLocation() + dir * speed * GetWorld()->DeltaTimeSeconds;
				SetActorLocation(newLocation);
			}
			else
			{
				flyingPattern = 0;
			}
			break;
		}

	}
}

void AEnemy_Dragon::Grounding()
{
	//땅으로 내려오기
	FVector targetLocation = target->GetActorLocation();
	FVector currentLocation = GetActorLocation();
	FVector dir;
	FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);
	FRotator newRotation = GetActorRotation();
	FVector newLocation;

	if (isFlying)
	{
		dir = targetPrevLocation - currentLocation;
		//player가 있는 방향으로 부드럽게 회전
		if (FMath::Abs(GetActorRotation().Yaw - lookRotation.Yaw) > 1)
		{
			if (GetActorRotation().Yaw > lookRotation.Yaw)
				newRotation.Yaw -= 2;
			else if (GetActorRotation().Yaw <= lookRotation.Yaw)
				newRotation.Yaw += 2;

			SetActorRotation(newRotation);
		}
		else
		{
			//타겟의 이전위치로 날아오기
			dir.Normalize();
			newLocation = GetActorLocation() + dir * speed * GetWorld()->DeltaTimeSeconds;

			if (targetPrevLocation.Z < GetActorLocation().Z)
				SetActorLocation(newLocation);
			else
			{
				// 				PhysicsComp->SetSimulatePhysics(true);//시간이 짧으면 도착 못할 수도 있으므로 충분히 길게 줄 것? 시간이 아니고 bool값으로 판단해야 하나?
				PhysicsComp->GetBodyInstance()->bLockXRotation = true;
				PhysicsComp->GetBodyInstance()->bLockYRotation = true;

				isFlying = false;

			}
		}
	}
	else
	{
		currentWakeUpDelay += GetWorld()->DeltaTimeSeconds;
		if (currentWakeUpDelay > wakeUpDelay)
			isWalking = true;
		//공격범위에 있을 땐 다가가지 않기

		if (!isBiteAttack)
		{
			SetActorRotation(lookRotation);
			//MoveToTarget

			dir = targetLocation - currentLocation;
			dir.Normalize();
			newLocation = GetActorLocation() + dir * speed / 5 * GetWorld()->DeltaTimeSeconds;
			SetActorLocation(newLocation);
			isClawAttack = false;
			currentClawAttackTime += GetWorld()->DeltaTimeSeconds;

			if (currentClawAttackTime > clawAttackTime)
			{
				isClawAttack = true;
				//Spawn claw
				currentClawAttackTime = 0;
				isClawSpawn = true;
			}
			if (ClawClass && isClawSpawn)
			{
				clawSpawnTime += GetWorld()->DeltaTimeSeconds;
				FActorSpawnParameters params;
				params.Owner = this;
				params.Instigator = GetInstigator();
				UE_LOG(LogTemp, Warning, TEXT("claw Spawn time %f"), clawSpawnTime);

				if (clawSpawnTime > 0.8 && clawSpawnTime < 0.9)
				{
					AEnemy_BulletBall* Claw = GetWorld()->SpawnActor<AEnemy_BulletBall>(ClawClass, ClawPosition2->GetComponentTransform(), params);

				}
				if (clawSpawnTime > 1.5 && clawSpawnTime < 1.6)
				{//claw의 spawn 타이밍..
					AEnemy_BulletBall* Claw = GetWorld()->SpawnActor<AEnemy_BulletBall>(ClawClass, ClawPosition1->GetComponentTransform(), params);
					clawSpawnTime = 0;
					isClawSpawn = false;
				}

			}

		}
	}
}



void AEnemy_Dragon::OnHitOverlap_Begin(AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapHit"));

	AH_PlayerBullet* bullet = Cast<AH_PlayerBullet>(OtherActor);
	ASkillAttack1* skillAttack = Cast<ASkillAttack1>(OtherActor);
	if (OtherActor != this && !isDead)
	{
		if (OtherActor == bullet && isInteracting)
		{
			UE_LOG(LogTemp, Warning, TEXT("bulletHit"));
			isHit = true;
			dragonHP -= bullet->GetBulletDamage();
			bullet->ExploreEffect_Dragon();

		}
		if (OtherActor == skillAttack && isInteracting)
		{
			isHit = true;
			dragonHP -= skillAttack->GetAttackDamage();
		}
	}
}

void AEnemy_Dragon::OnHitOverlap_End(AActor* OtherActor)
{
	AH_PlayerBullet* bullet = Cast<AH_PlayerBullet>(OtherActor);
	ASkillAttack1* skillAttack = Cast<ASkillAttack1>(OtherActor);
	if (OtherActor != this && !isDead)
	{
		if (OtherActor == bullet || skillAttack)
		{
			isHit = false;
		}
	}
}

void AEnemy_Dragon::OnAttackOverlap_Begin(AActor* OtherActor)
{
	if (OtherActor != this && OtherActor == target && !isDead)
		isBiteAttack = true;
}

void AEnemy_Dragon::OnAttackOverlap_End(AActor* OtherActor)
{
	if (OtherActor != this && OtherActor == target && !isDead)
		isBiteAttack = false;
}


/// <summary>
/// Getter Setter
/// </summary>
/// <returns></returns>
void AEnemy_Dragon::ResetSettings()
{
	isSpreading = false;
	isSpitting = false;
	isAttackActivate = false;
	isGliding = false;
	isWakeUp = true;
	isWalking = false;
	isResetPitch = false;
	currentWakeUpDelay = 0.0f;
	currentPatternTime = 0;
}

bool AEnemy_Dragon::GetIsFlying()
{
	return isFlying;
}

void AEnemy_Dragon::SetIsInteraction(bool input)
{
	isInteracting = input;
}

void AEnemy_Dragon::SetIsWakeUp(bool input)
{
	isWakeUp = input;
}

bool AEnemy_Dragon::GetIsSpitting()
{
	return isSpitting;
}

bool AEnemy_Dragon::GetIsSpreading()
{
	return isSpreading;
}


bool AEnemy_Dragon::GetIsHit()
{
	return isHit;
}

bool AEnemy_Dragon::GetIsWalking()
{
	return isWalking;
}

bool AEnemy_Dragon::GetIsBite()
{
	return isBiteAttack;
}

bool AEnemy_Dragon::GetIsGliding()
{
	return isGliding;
}

bool AEnemy_Dragon::GetIsDead()
{
	return isDead;
}

bool AEnemy_Dragon::GetIsClaw()
{
	return isClawAttack;
}

int AEnemy_Dragon::GetBiteAttackDamage()
{
	return biteAttackDamage;
}

bool AEnemy_Dragon::GetIsCineCamera()
{
	return isCineCamera;
}

void AEnemy_Dragon::SetIsCineCamera(bool input)
{
	isCineCamera = input;
}

// Called to bind functionality to input
void AEnemy_Dragon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

