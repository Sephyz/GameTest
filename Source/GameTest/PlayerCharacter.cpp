// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Inizialize components
	m_HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	// Setup camera boom
	m_CameraBoom->SetupAttachment(RootComponent);
	m_CameraBoom->bUsePawnControlRotation = true;

	// Setup camera
	m_Camera->SetupAttachment(m_CameraBoom);

	//Setting class variables of the Character movement component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;



}

void APlayerCharacter::MoveSideways(float MovementDelta)
{
	if ( Controller != NULL && MovementDelta != 0.0f)
	{
		// Find out which direction is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, MovementDelta);
	}
}

void APlayerCharacter::MoveForward(float MovementDelta)
{
	if (Controller != NULL && MovementDelta != 0.0f)
	{
		// Find out which direction is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, MovementDelta);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_HealthComponent->OnHealthChanged.AddDynamic(this, &APlayerCharacter::OnHealthChanged);
}

void APlayerCharacter::BeginSprint()
{
	if (!m_IsBlocking)
	{
		m_IsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	}
}

void APlayerCharacter::EndSprint()
{
	if (m_IsSprinting)
	{
		m_IsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

void APlayerCharacter::BeginAttack()
{
	if (m_AttackCooldown <= 0.0f)
	{
		m_AttackCooldown = 0.5f;
	}
}

void APlayerCharacter::BeginBlock()
{
	if (!m_IsSprinting)
	{
		m_IsBlocking = true;
		GetCharacterMovement()->MaxWalkSpeed = 100.f;
	}
}

void APlayerCharacter::EndBlock()
{
	if (m_IsBlocking)
	{
		m_IsBlocking = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

void APlayerCharacter::OnHealthChanged(UHealthComponent* HealthComp, float Health, float DamageAmount, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f)
	{
		Destroy();
	}
}

void APlayerCharacter::DamagePlayerCharacter(float DamageAmount)
{
	UGameplayStatics::ApplyDamage(this, DamageAmount, GetInstigatorController(), this, m_GenericDamageType);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_AttackCooldown > 0.0f)
	{
		m_AttackCooldown -= DeltaTime;
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Register input
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::BeginSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::EndSprint);


	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::BeginAttack);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &APlayerCharacter::BeginBlock);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &APlayerCharacter::EndBlock);

	PlayerInputComponent->BindAxis("MoveLR", this, &APlayerCharacter::MoveSideways);
	PlayerInputComponent->BindAxis("MoveFB", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("TurnCameraX", this, &APawn::AddControllerYawInput);
}

