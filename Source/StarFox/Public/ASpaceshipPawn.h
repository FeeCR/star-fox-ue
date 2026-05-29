// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ASpaceshipPawn.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UStaticMeshComponent;
class UCameraComponent;
class UFloatingPawnMovement;

UCLASS()
class STARFOX_API AASpaceshipPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AASpaceshipPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> MappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USceneComponent> RootSceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UFloatingPawnMovement> MovementComponent;
	
	//Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float ForwardSpeed = 1200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float LateralSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float BoundaryY = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float BoundaryZ = 400.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilt")
	float MaxRollAngle = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilt")
	float MaxPitchAngle = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilt")
	float TiltInterpSpeed = 8.f;
	
private:
	void ApplyMovement(float DeltaTime);
	void ApplyTilt(float DeltaTime);
	
	FVector2D CurrentInput = FVector2D::ZeroVector;
	FRotator CurrentMeshRotation = FRotator::ZeroRotator;
	
	void OnMove(const FInputActionValue& Value);
	void OnMoveCompleted(const FInputActionValue& Value);
	
};
