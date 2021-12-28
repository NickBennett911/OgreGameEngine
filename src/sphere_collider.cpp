#include <stdafx.h>
#include <sphere_collider.h>
#include <game_object.h>

ssuge::SphereCollider::SphereCollider(ssuge::GameObject* owner, float radius, float mass, bool is_kinematic) : mMass(mass), mIs_kinematic(is_kinematic), Collider(owner) {
	mCollisionBody = new btSphereShape(btScalar(radius));

	// set up the colliders transform to be the same as the parents position at the start
	btTransform mTransform;
	mTransform.setIdentity();
	Ogre::Vector3 pos = owner->get_position();
	mTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		mCollisionBody->calculateLocalInertia(mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	myMotionState = new btDefaultMotionState(mTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, mCollisionBody, localInertia);
	mRigidBody = new btRigidBody(rbInfo);

	PHYSICS_MANAGER->add_ridgid_body(mRigidBody);

	mCollisionBody->setUserPointer(this);
	mRigidBody->setUserPointer(this);
}

ssuge::SphereCollider::~SphereCollider() {
	delete mCollisionBody;
	delete mRigidBody;
	delete myMotionState;
}

void ssuge::SphereCollider::update(float dt) {
	SphereCollider* spherec = (SphereCollider*)mCollisionBody->getUserPointer();
	GameObject* gobj = spherec->get_owner();

	btTransform trans = mRigidBody->getWorldTransform();
	btVector3 pos = trans.getOrigin();
	btQuaternion orien = trans.getRotation();
	Ogre::Quaternion quat = Ogre::Quaternion((float)orien.getW(), (float)orien.getX(), (float)orien.getY(), (float)orien.getZ());
	if (!mIs_kinematic) {
		gobj->set_position_world((float)pos.getX(), (float)pos.getY(), (float)pos.getZ());
		gobj->set_orientation(quat);
	}
	else {
		//get and set positional info
		Ogre::Vector3 gobjpos = gobj->get_world_position();
		btTransform newt = mRigidBody->getCenterOfMassTransform();
		newt.setOrigin(btVector3(gobjpos.x, gobjpos.y, gobjpos.z));
		//get and set rotational info
		Ogre::Quaternion gobjquat = gobj->get_world_orientation();// Get gobj ogre quaternion
		Ogre::Degree deg;
		Ogre::Vector3 vec;
		gobjquat.ToAngleAxis(deg, vec);	// get the degree and vector values from that quaternion
		btVector3 newaxis = btVector3(vec.x, vec.y, vec.z);	// create the btVector form that Ogre vector
		btQuaternion newq = btQuaternion(newaxis, deg.valueRadians());	// create the btQuaternion from the extracted infromatino
		newt.setRotation(newq);		// add that quaternion to the new transfrom

		mRigidBody->setCenterOfMassTransform(newt);	// add the new transfrom to the center of mass

	}
}