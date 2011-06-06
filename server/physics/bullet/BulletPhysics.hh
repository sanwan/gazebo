/*
 *  Gazebo - Outdoor Multi-Robot Simulator
 *  Copyright (C) 2003  
 *     Nate Koenig & Andrew Howard
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
/* Desc: The Bullet physics engine wrapper
 * Author: Nate Koenig
 * Date: 11 June 2009
 * SVN: $Id: BulletPhysics.hh 7706 2009-05-21 16:46:22Z natepak $
 */

#ifndef BULLETPHYSICS_HH
#define BULLETPHYSICS_HH

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "Param.hh"
#include "PhysicsEngine.hh"
#include "Geom.hh"
#include "Shape.hh"

namespace gazebo
{
  class Entity;
  class XMLConfigNode;
  class Mass;

/// \addtogroup gazebo_physics_engine
/// \{

/** \defgroup bulletphysicsengine Bullet Physics Engine

The \c param:physics tag is used to specify certain parameters for the Bullet phyics engine. The following parameters are in addition to those provided by the PhysicsEngine base class.

\par Attributes

- cfm (float)
  - Global constraint force mixing
  - Default: 10e-5
  - Range:  10e-10 to 1.0
  - Recommended value: 10e-5
- erp (float)
  - Global error reduction parameter
  - Default: 0.2
  - Range: 0 to 1.0
  - Recommended Range: 0.1 to 0.8
- stepTime (float)
  - Time, in seconds, that elapse for each iteration of the physics engine
  - Default: 0.025
-gravity (float float float)
  - Gravity vector.
  - Default: 0 0 -9.8

\verbatim
<physics:bullet>
  <stepTime>0.03</stepTime>
  <gravity>0 0 -9.8</gravity>
  <cfm>10e-5</cfm>
  <erp>0.2</erp>
</physcis:bullet>
\endverbatim

\{
*/

/// \brief Bullet physics engine
class BulletPhysics : public PhysicsEngine
{
  /// \brief Constructor
  public: BulletPhysics();

  /// \brief Destructor
  public: virtual ~BulletPhysics();

  /// \brief Load the Bullet engine
  public: virtual void Load(XMLConfigNode *node);

  /// \brief Saves to XMLFile
  public: void Save(std::string &prefix, std::ostream &stream);

  /// \brief Initialize the Bullet engine
  public: virtual void Init();

  /// \brief Initialize for separate thread
  public: virtual void InitForThread();
 
  /// \brief Update the Bullet collision
  public: virtual void UpdateCollision();

  /// \brief Update the Bullet engine
  public: virtual void UpdatePhysics();

  /// \brief Finilize the Bullet engine
  public: virtual void Fini();

  /// \brief Add an entity to the world
  public: void AddEntity(Entity *entity);

  /// \brief Remove an entity from the physics engine
  public: virtual void RemoveEntity(Entity *entity);

  /// \brief Create a new body
  public: virtual Body *CreateBody(Entity *parent);

  /// \brief Create a new geom
  public: virtual Geom *CreateGeom(Shape::Type type, Body *body);

  /// \brief Create a new joint
  public: virtual Joint *CreateJoint(Joint::Type type);

  /// \brief Create a physics based ray sensor
  //public: virtual PhysicsRaySensor *CreateRaySensor(Body *body);
 
  /// \brief Convert an bullet mass to a gazebo Mass
  public: virtual void ConvertMass(Mass *mass, void *engineMass);

  /// \brief Convert an gazebo Mass to a bullet Mass
  public: virtual void ConvertMass(void *engineMass, const Mass &mass);

  /// \brief Convert a bullet transform to a gazebo pose
  public: static Pose3d ConvertPose(btTransform bt);

  /// \brief Convert a gazebo pose to a bullet transform
  public: static btTransform ConvertPose(const Pose3d pose);

  /// \brief Register a joint with the dynamics world
  public: btDynamicsWorld *GetDynamicsWorld() const
          {return this->dynamicsWorld;}

  /// \brief stores rms errors from quickstep
  public: std::vector<double> rms_error;
  public: std::vector<double>::iterator rms_error_iter;
  public: double GetRMSErrorTolerance();
  public: double GetRMSError();

  //private: btAxisSweep3 *broadPhase;
  private: btBroadphaseInterface *broadPhase;
  private: btDefaultCollisionConfiguration *collisionConfig;
  private: btCollisionDispatcher *dispatcher;
  private: btSequentialImpulseConstraintSolver *solver;
  private: btDiscreteDynamicsWorld *dynamicsWorld;

  private: Time lastUpdateTime;
};

/** \}*/
/// \}
}

#endif
