pipeline {
    agent any  // Use local Jenkins agent instead of Docker
    
    environment {
        DOCKER_HUB_CREDENTIALS = credentials('docker-hub-credentials')
        // Set Python path for macOS (adjust if needed)
        PATH = "/usr/local/bin:/opt/homebrew/bin:$PATH"
    }
    
    stages {
        stage('Checkout') {
            steps {
                git url: 'https://github.com/mehoolgoria1020/pqc_nuig.git', branch: 'main'
            }
        }
        
        stage('Setup Python Environment') {
            steps {
                sh '''
                    # Check Python version
                    python3 --version || python --version
                    
                    # Create virtual environment
                    python3 -m venv venv || python -m venv venv
                    
                    # Activate virtual environment and upgrade pip
                    source venv/bin/activate
                    pip install --upgrade pip
                '''
            }
        }
        
        stage('Install Dependencies') {
            steps {
                sh '''
                    source venv/bin/activate
                    pip install --no-cache-dir -r requirements.txt
                '''
            }
        }
        
        stage('Test') {
            steps {
                sh '''
                    source venv/bin/activate
                    python -m unittest discover -s tests
                '''
            }
        }
        
        stage('Build Docker Image') {
            steps {
                sh '''
                    # Ensure Docker is running
                    docker --version
                    docker info
                    
                    # Build the Docker image
                    docker build -t 9977262607/pqc-flask-demo:latest .
                '''
            }
        }
        
        stage('Push Docker Image') {
            steps {
                withCredentials([usernamePassword(credentialsId: 'docker-hub-credentials', usernameVariable: 'DOCKER_USER', passwordVariable: 'DOCKER_PASS')]) {
                    sh '''
                        echo $DOCKER_PASS | docker login -u $DOCKER_USER --password-stdin
                        docker push 9977262607/pqc-flask-demo:latest
                        docker logout
                    '''
                }
            }
        }
    }
    
    post {
        always {
            // Clean up virtual environment
            sh '''
                if [ -d "venv" ]; then
                    rm -rf venv
                fi
            '''
            
            // Clean up Docker images to save space (optional)
            sh '''
                docker image prune -f || echo "Docker image cleanup failed"
            '''
            
            // Clean workspace
            cleanWs()
        }
        
        success {
            echo 'Pipeline completed successfully!'
            sh 'echo "Docker images:" && docker images | grep pqc-flask-demo || echo "No pqc-flask-demo images found"'
        }
        
        failure {
            echo 'Pipeline failed!'
            // Keep logs for debugging
            sh '''
                echo "=== Docker Status ==="
                docker --version || echo "Docker not available"
                docker info || echo "Docker daemon not running"
                
                echo "=== Python Status ==="
                python3 --version || python --version || echo "Python not available"
                
                echo "=== Current Directory ==="
                ls -la
                
                echo "=== Requirements File ==="
                cat requirements.txt || echo "requirements.txt not found"
            '''
        }
    }
}
