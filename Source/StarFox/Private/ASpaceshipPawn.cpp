// Fill out your copyright notice in the Description page of Project Settings.


#include "ASpaceshipPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AASpaceshipPawn::AASpaceshipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(RootSceneComponent);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(RootSceneComponent);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootSceneComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 80.0f);
	SpringArm->SetRelativeRotation(FRotator(-10.0f, 0.0f, 0.0f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 6.f;
	
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
	MovementComponent->MaxSpeed = LateralSpeed;

}

// Called when the game starts or when spawned
void AASpaceshipPawn::BeginPlay()
{
	Super::BeginPlay();
	
	MeshBaseRotation = MeshComponent->GetRelativeRotation();

	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

// Called every frame
void AASpaceshipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddActorWorldOffset(FVector(ForwardSpeed * DeltaTime, 0.f, 0.f), true);

	ApplyMovement(DeltaTime);
	ApplyTilt(DeltaTime);

}

// Called to bind functionality to input
void AASpaceshipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC) return;

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AASpaceshipPawn::OnMove);
	EIC->BindAction(MoveAction, ETriggerEvent::Completed, this, &AASpaceshipPawn::OnMoveCompleted);
}

void AASpaceshipPawn::ApplyMovement(float DeltaTime)
{
	const FVector Delta(
		0.f,
		CurrentInput.X * LateralSpeed * DeltaTime,
		CurrentInput.Y * LateralSpeed * DeltaTime
		);
	AddActorWorldOffset(Delta, true);
	
	FVector Loc = GetActorLocation();
	Loc.Y = FMath::Clamp(Loc.Y, -BoundaryY, BoundaryY);
	Loc.Z = FMath::Clamp(Loc.Z, -BoundaryZ, BoundaryZ);
	SetActorLocation(Loc);
}

void AASpaceshipPawn::ApplyTilt(float DeltaTime)
{
	const FRotator Target(
		 -CurrentInput.X * MaxPitchAngle,
		 180.f,
		CurrentInput.Y * MaxRollAngle
	);
	
	
	
	CurrentMeshRotation = FMath::RInterpTo(
		CurrentMeshRotation,
		Target,
		DeltaTime,
		TiltInterpSpeed
	);

	MeshComponent->SetRelativeRotation(MeshBaseRotation + CurrentMeshRotation);
}

void AASpaceshipPawn::OnMove(const FInputActionValue& Value)
{
	CurrentInput = Value.Get<FVector2D>();
}

void AASpaceshipPawn::OnMoveCompleted(const FInputActionValue& Value)
{
	CurrentInput = FVector2D::ZeroVector;
}

