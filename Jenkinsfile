pipeline {
    agent {
        docker {
            image 'python:3.9-slim'
            args '-u root'
        }
    }
    environment {
        DOCKER_HUB_CREDENTIALS = credentials('docker-hub-credentials')
    }
    stages {
        stage('Checkout') {
            steps {
                git url: 'https://github.com/mehoolgoria1020/pqc_nuig', branch: 'main'
            }
        }
        stage('Install Dependencies') {
            steps {
                sh 'pip install --no-cache-dir -r requirements.txt'
            }
        }
        stage('Test') {
            steps {
                sh 'python -m unittest discover -s tests'
            }
        }
        stage('Build Docker Image') {
            steps {
                sh 'docker build -t 9977262607/pqc-flask-demo:latest .'
            }
        }
        stage('Push Docker Image') {
            steps {
                withCredentials([usernamePassword(credentialsId: 'docker-hub-credentials', usernameVariable: 'DOCKER_USER', passwordVariable: 'DOCKER_PASS')]) {
                    sh 'echo $DOCKER_PASS | docker login -u $DOCKER_USER --password-stdin'
                    sh 'docker push 9977262607/pqc-flask-demo:latest'
                }
            }
        }
    }
    post {
        always {
            cleanWs()
        }
    }
}
