// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//chapter 10 ����
/*
	�÷��̾ �����ϴ� �ݸ��� �ڽ��� ������ ���ڸ� �ð�ȭ���ִ� ����ƽ�޽÷� ������
	��Ʈ ������Ʈ�� �÷��̾�� �� ��ħ�� ������ �ڽ� �ݸ��� ������Ʈ�� ����ϰ� �ڽĿ��� ����ƽ�޽� ������Ʈ�� �߰��Ѵ�

	/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1

	�������� ����
	����� �÷��̾�� �������� ����ִ� ���
	��ġ�� ���ڿ� Ŭ���� ������ ������ �Ӽ��� �߰�, �÷��̾ �̿��� ������ ������ ������ �� �������� �����ϵ��� ����� ����

	Ŭ���� ������ ������ ��, ������ UCLASS�� �����ص� ������ �̸� ����ϸ�,
	���� ������Ʈ�� ����ϴ� ��� �𸮾� ������Ʈ�� ������ ���̰� �ȴ�
	Ư�� Ŭ������ ��ӹ��� Ŭ������� ����� �����ϴ� TSubclassof ��� Ű���带 �����Ѵ�
	�̸� ����ϸ� ��Ͽ��� ������ ���ڿ� �̸� ������ Ŭ���� ��ϸ� �� �� �ִ�.

*/

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"


UCLASS()
class ARENABATTLE_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//chapter 10, make box can scan player pawn to do something
	virtual void PostInitializeComponents() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	
	UPROPERTY(VisibleAnywhere, Category = Box)
		UBoxComponent*Trigger;//����������?
	UPROPERTY(VisibleAnywhere, Category = Box)
		UStaticMeshComponent* Box;
	//chapter 10
	//TSubvlassOf�� �̿��Ͽ� AABWeapon�� ������ ����
	UPROPERTY(EditInstanceOnly, Category = Box)
		TSubclassOf<class AABWeapon> WeaponItemClass;
	//chaptert 10 add box effect
	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* Effect;

	//chapter 10
private:
	UFUNCTION()
		void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,const  FHitResult& SweepResult);
	UFUNCTION()
		void OnEffectFinished(class UParticleSystemComponent*PSystem);
};
