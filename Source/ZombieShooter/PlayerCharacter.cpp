// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h" 
#include "Net/UnrealNetwork.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto mesh = GetMesh();
	TArray<FName> names;
	mesh->GetBoneNames(names);
	for (auto& n : names)
		UE_LOG(LogTemp, Warning, TEXT("Bone: %s"), *n.ToString());
}

void APlayerCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void APlayerCharacter::OnHealthUpdate()
{
	if (IsLocallyControlled()) {
		FString healthMsg = FString::Printf(TEXT("Health: %f"), currentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, healthMsg);
		if (currentHealth <= 0) 
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You dead.")));
	}

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerCharacter::SetCurrentHealth(float value)
{
	if (GetLocalRole() != ROLE_Authority)
		return;
	currentHealth = FMath::Clamp(value, 0.f, maxHealth);
	OnHealthUpdate();
}

float APlayerCharacter::TakeDamage(float dmgTaken, FDamageEvent const& dmgEvet, AController* eventInstigator, AActor* dmgCauser)
{
	float newHp = currentHealth - dmgTaken;
	SetCurrentHealth(newHp);
	return newHp;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, currentHealth);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("PlayerMoveFwd", this, &APlayerCharacter::MoveX);
	PlayerInputComponent->BindAxis("PlayerMoveRight", this, &APlayerCharacter::MoveY);
	PlayerInputComponent->BindAction("PlayerJump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PlayerJump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);
}

void APlayerCharacter::MoveX(float value)
{
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(direction, value);
}

void APlayerCharacter::MoveY(float value)
{
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(direction, value);
}