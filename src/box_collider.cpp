#include <stdafx.h>
#include <box_collider.h>
#include <game_object.h>

ssuge::BoxCollider::BoxCollider(ssuge::GameObject* owner, float xd, float yd, float zd, float mass, bool is_kinematic) : mMass( mass ), mIs_kinematic( is_kinematic ), Collider(owner) {

	mCollisionBody = new btBoxShape(btVector3(btScalar(xd), btScalar(yd), btScalar(zd)));

	// set up the colliders transform to be the same as the parents position at the start
	btTransform mTransform;
	mTransform.setIdentity();
	Ogre::Vector3 pos = owner->get_position();
	mTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	if (is_kinematic) {
		mass = 0.f;
		mMass = 0.f;
	}
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

ssuge::BoxCollider::~BoxCollider() {
	PHYSICS_MANAGER->remove_rigid_body(mRigidBody);
	delete mCollisionBody;
	delete mRigidBody;
	delete myMotionState;
}

void ssuge::BoxCollider::update(float dt) {
	BoxCollider* boxc = (BoxCollider*)mCollisionBody->getUserPointer();
	GameObject* gobj = boxc->get_owner();

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