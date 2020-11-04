// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ZOMBIESHOOTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float maxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
		float currentHealth;

	UFUNCTION()
		void OnRep_CurrentHealth();

	void OnHealthUpdate();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return maxHealth; }
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return currentHealth; }

	//UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE void SetCurrentHealth(float value);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float dmgTaken, struct FDamageEvent const& dmgEvent, AController* eventInstigator, AActor* dmgCauser) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveX(float value);

	void MoveY(float value);

};
