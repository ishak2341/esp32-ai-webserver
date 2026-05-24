// ===== Chat Application JavaScript =====

const API_BASE_URL = '';
const chatMessages = document.getElementById('chatMessages');
const messageInput = document.getElementById('messageInput');
const sendBtn = document.getElementById('sendBtn');
const loadingIndicator = document.getElementById('loadingIndicator');
const toast = document.getElementById('toast');

// ===== Event Listeners =====
sendBtn.addEventListener('click', sendMessage);
messageInput.addEventListener('keypress', (e) => {
    if (e.key === 'Enter' && !e.shiftKey) {
        e.preventDefault();
        sendMessage();
    }
});

// ===== Send Message Function =====
async function sendMessage() {
    const message = messageInput.value.trim();
    
    if (!message) {
        showToast('Lütfen bir mesaj yazın', 'warning');
        return;
    }
    
    // Add user message to chat
    addMessage(message, 'user');
    messageInput.value = '';
    messageInput.focus();
    
    // Show loading indicator
    showLoading(true);
    
    try {
        // Call API
        const response = await fetch(`${API_BASE_URL}/api/chat`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                message: message
            })
        });
        
        if (!response.ok) {
            throw new Error(`HTTP Error: ${response.status}`);
        }
        
        const data = await response.json();
        
        if (data.success) {
            // Add AI response to chat
            addMessage(data.ai_response, 'ai');
            showToast('Yanıt alındı', 'success');
        } else {
            throw new Error(data.error || 'Unknown error');
        }
    } catch (error) {
        console.error('Error:', error);
        addMessage(
            `Hata: ${error.message}. Lütfen API anahtarınızı kontrol edin ve WiFi bağlantınızdan emin olun.`,
            'ai'
        );
        showToast('İstek başarısız oldu', 'error');
    } finally {
        showLoading(false);
    }
}

// ===== Add Message to Chat =====
function addMessage(text, sender) {
    const messageDiv = document.createElement('div');
    messageDiv.classList.add('message', sender);
    
    const avatar = document.createElement('div');
    avatar.classList.add('message-avatar');
    
    if (sender === 'user') {
        avatar.innerHTML = '<i class="fas fa-user"></i>';
    } else {
        avatar.innerHTML = '<i class="fas fa-robot"></i>';
    }
    
    const contentDiv = document.createElement('div');
    contentDiv.classList.add('message-content');
    
    const paragraph = document.createElement('p');
    paragraph.textContent = text;
    
    contentDiv.appendChild(paragraph);
    messageDiv.appendChild(avatar);
    messageDiv.appendChild(contentDiv);
    
    chatMessages.appendChild(messageDiv);
    
    // Scroll to bottom
    chatMessages.scrollTop = chatMessages.scrollHeight;
}

// ===== Show/Hide Loading Indicator =====
function showLoading(show) {
    if (show) {
        loadingIndicator.classList.remove('hidden');
    } else {
        loadingIndicator.classList.add('hidden');
    }
}

// ===== Show Toast Notification =====
function showToast(message, type = 'info') {
    toast.textContent = message;
    toast.classList.remove('hidden');
    
    // Auto hide after 3 seconds
    setTimeout(() => {
        toast.classList.add('hidden');
    }, 3000);
}

// ===== Initialize App =====
function initializeApp() {
    console.log('AI Assistant initialized');
    checkDeviceStatus();
}

// ===== Check Device Status =====
async function checkDeviceStatus() {
    try {
        const response = await fetch(`${API_BASE_URL}/api/status`);
        const data = await response.json();
        console.log('Device Status:', data);
        
        if (data.status === 'online') {
            showToast('Cihaz bağlı', 'success');
        }
    } catch (error) {
        console.error('Status check failed:', error);
    }
}

// ===== Startup =====
window.addEventListener('load', initializeApp);
