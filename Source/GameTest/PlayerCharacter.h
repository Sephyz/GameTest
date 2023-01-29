// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class GAMETEST_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void BeginSprint();
	void EndSprint();
	void BeginAttack();
	void EndAttack();
	void BeginBlock();
	void EndBlock();

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* HealthComp, float Health, float DamageAmount, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	//Function which will call damage to our Player
	UFUNCTION()
	void DamagePlayerCharacter(float DamageAmount);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UHealthComponent* m_HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* m_Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* m_CameraBoom;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	bool m_IsAttacking = false;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	float m_AttackCooldown = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	bool m_IsBlocking = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	bool m_IsSprinting = false;

	//Container for a Damage Type to inflict on the Player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> m_GenericDamageType;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Character Input
	void MoveSideways(float MovementDelta);
	void MoveForward(float MovementDelta);
};
